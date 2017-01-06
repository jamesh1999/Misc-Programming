#ifndef __TRACK_TYPE_INCLUDED__
#define __TRACK_TYPE_INCLUDED__

namespace ConfigData
{
	typedef struct
	{
		double costPerLength;
		double repulsionForce;
		int obstructionID;
	} TrackType;
}

#endif