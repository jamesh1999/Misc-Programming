#ifndef __MESHDATA_H__
#define __MESHDATA_H__

#include <DirectXMath.h>
#include <vector>

struct Vertex
{
	DirectX::XMFLOAT3A pos;
	DirectX::XMFLOAT3A normal;
	DirectX::XMFLOAT2A tex;
};

struct MeshData
{
	std::vector<Vertex> vertices;
	std::vector<int> indices;
};

#endif