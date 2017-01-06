#ifndef __TRAIN_TYPE_INCLUDED__
#define __TRAIN_TYPE_INCLUDED__

namespace ConfigData
{
	typedef struct
	{
		double seatInclination;
		double accelerationForce;
		double cD;
		double mass;
		double runningCost;
		double initialCost;
		double decellerationForce;
		int capacity;
		bool variableSeatInclination;
	} TrainType;
}

#endif