#ifndef __VERTEX_INCLUDED__
#define __VERTEX_INCLUDED__

#include <d3d11.h>
#include <DirectXMath.h>

typedef struct
{
	FLOAT x, y, z;
	DirectX::XMVECTORF32 Color;
} VERTEX;

#endif