#include "stdafx.h"
#include "Simulation.h"
#include "Config.h"

float CheckSpeed(std::vector<SpeedControl> speedplan, Route track, float deltaT)
{
	double speed = 0.0f;
	double dist = 0.0f;
	double tot_dist = 0.0f;
	double tot_segmented_dist = 0.0f;
	double t = 0.0f;
	int spd = 1;
	SpeedControl::Type type = speedplan[0].type;
	for (int i = 0; i < track.size(); ++i)
	{
		do
		{
			double a;

			while (speedplan.size() > spd && speedplan[spd].dist < tot_dist)
				type = speedplan[spd++].type;

			switch(type)
			{
			case SpeedControl::Type::Accel:
				a =
					(Config::instance->trains[0].accelerationForce
						- speed * speed
						* Config::instance->trains[0].cD)
					/ Config::instance->trains[0].mass;
				break;
			case SpeedControl::Type::Decel:
				a =
					(-Config::instance->trains[0].decellerationForce
						- speed * speed
						* Config::instance->trains[0].cD)
					/ Config::instance->trains[0].mass;
				break;
			case SpeedControl::Type::Hold:
				a = 0.0;
				speed = speedplan[spd - 1].speed;
			}

			double oldspeed = speed;
			speed += deltaT * a;

			if (speed < 0.0)
				return -tot_dist;

			//Check within limit
			if (track[i].radius > 0.0)
			{
				double centripetal_acc = speed * speed / track[i].radius;

				if (centripetal_acc > Config::instance->maxGForceX
					|| centripetal_acc * Config::instance->trains[0].mass > Config::instance->tracks[0].repulsionForce)
					return -tot_dist + dist;
			}

			dist += (speed + oldspeed) / 2 * deltaT;
			tot_dist += (speed + oldspeed) / 2 * deltaT;
			t += deltaT;
		} while (dist < track[i].length);

		dist -= track[i].length;
	}

	return t;
}

Result SimulateTrack(Route track, float deltaT, float deltaS, int track_id, int train_id)
{
	std::vector<SpeedControl> speedplan;
	speedplan.push_back({ 0.0f, 0.0f, SpeedControl::Type::Accel });
	int cur = 0;

	double totLength = 0.0;
	for (int i = 1; i < track.size() - 1; ++i)
	{
		totLength += track[i].length;
	}

	while (true)
	{
		float val = CheckSpeed(speedplan, track, deltaT);

		if (val > 0.0)
			return{ 0, 0, val, speedplan };

		speedplan.push_back({ -val - deltaS, 0.0f, SpeedControl::Type::Decel });
		
		float test = CheckSpeed(speedplan, track, deltaT);
		while (test < 0.0
			&& test >= val)
		{
			double d = speedplan[speedplan.size() - 1].dist - deltaS;
			for (int i = 0; i < speedplan.size(); ++i)
			{
				if(speedplan[i].dist > d)
				{
					speedplan[i].dist = d;
					speedplan[i].type = SpeedControl::Type::Decel;
				}
			}

			test = CheckSpeed(speedplan, track, deltaT);
		}

		double acc = 0.0;
		for (int i = 0; i < track.size() - 1; ++i)
		{
			if (acc > -val + 0.01f)
			{

				float maxSpeed = std::sqrtf(Config::instance->tracks[0].repulsionForce / Config::instance->trains[0].mass * track[i].radius) * 0.95f;

				speedplan.push_back({ (float)acc - (float)track[i - 1].length, maxSpeed, SpeedControl::Type::Hold });

				acc += track[i].length;
				speedplan.push_back({ (float)acc, 0.0f, SpeedControl::Type::Accel });

				break;
			}

			acc += track[i].length;
		}
	}
}

std::vector<Result> Simulate(Route route, int resolution)
{
	std::vector<Result> ret;
	ret.push_back(SimulateTrack(route, 0.5, 5.0, 0, 0));

	return ret;
}