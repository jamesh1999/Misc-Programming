#ifndef __MESH_LOADER_INCLUDED__
#define __MESH_LOADER_INCLUDED__

#include <string>
#include <fbxsdk.h>
#include "MeshData.h"

class MeshLoader
{
private:
	static MeshLoader* instance;

	//FBX loader helper functions
	static void FBXGetVertexPos(FbxMesh*, int, DirectX::XMFLOAT3A&);
	static void FBXGetNormal(FbxMesh*, int, int, DirectX::XMFLOAT3A&);
	static void FBXGetUV(FbxMesh*, int, int, DirectX::XMFLOAT2A&);

public:
	MeshLoader();

	static void LoadFBX(MeshData*,std::string);
	static void LoadOBJ(MeshData*,std::string);
};

#endif