#ifndef __SIMULATION_INCLUDED__
#define __SIMULATION_INCLUDED__

#include <vector>
#include "Route.h"

typedef struct
{
	float dist;
	float speed;
	enum class Type {Accel, Decel, Hold};
	Type type;
	
} SpeedControl;

typedef struct
{
	int start;
	int end;
	float time;

	
	std::vector<SpeedControl> speedplan;
} Result;
std::vector<Result> Simulate(Route, int);


#endif

