#ifndef __GEOMETRY_INCLUDED__
#define __GEOMETRY_INCLUDED__

#include "MeshData.h"

namespace Geometry
{
	void GenerateCuboid(MeshData*, float, float, float);
	void GenerateSphere(MeshData*, float, int, int);
	void GenerateCylinder(MeshData*, float, float, float, int, int);
	void GenerateQuad(MeshData*, float, float, int, int);
}

#endif
