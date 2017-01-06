#ifndef __ROUTE_INCLUDED__
#define __ROUTE_INCLUDED__

#include <vector>

typedef struct
{
	double x;
	double y;
	double smoothness;
} InputSection;

typedef struct
{
	double radius;
	double length;
} Section;

typedef std::vector<Section> Route;

#endif