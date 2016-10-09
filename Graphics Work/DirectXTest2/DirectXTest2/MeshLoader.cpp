#include "MeshLoader.h"

MeshLoader* MeshLoader::instance = nullptr;

//FBX loader

void MeshLoader::FBXGetVertexPos(FbxMesh* in, int ctrlPointIdx, DirectX::XMFLOAT3A& out)
{
	out.x = in->GetControlPointAt(ctrlPointIdx).mData[0] / 100;
	out.y = in->GetControlPointAt(ctrlPointIdx).mData[1] / 100;
	out.z = in->GetControlPointAt(ctrlPointIdx).mData[2] / 100;
}

void MeshLoader::FBXGetNormal(FbxMesh* in, int ctrlPointIdx, int vtxIdx, DirectX::XMFLOAT3A& out)
{
	FbxGeometryElementNormal* norm = in->GetElementNormal(0);
	switch (norm->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (norm->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
		{
			int idx = norm->GetIndexArray().GetAt(ctrlPointIdx);
			out.x = norm->GetDirectArray().GetAt(idx).mData[0];
			out.y = norm->GetDirectArray().GetAt(idx).mData[1];
			out.z = norm->GetDirectArray().GetAt(idx).mData[2];
			break;
		}
		case FbxGeometryElement::eDirect:
			out.x = norm->GetDirectArray().GetAt(ctrlPointIdx).mData[0];
			out.y = norm->GetDirectArray().GetAt(ctrlPointIdx).mData[1];
			out.z = norm->GetDirectArray().GetAt(ctrlPointIdx).mData[2];
			break;
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (norm->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
		{
			int idx = norm->GetIndexArray().GetAt(vtxIdx);
			out.x = norm->GetDirectArray().GetAt(idx).mData[0];
			out.y = norm->GetDirectArray().GetAt(idx).mData[1];
			out.z = norm->GetDirectArray().GetAt(idx).mData[2];
			break;
		}
		case FbxGeometryElement::eDirect:
			out.x = norm->GetDirectArray().GetAt(vtxIdx).mData[0];
			out.y = norm->GetDirectArray().GetAt(vtxIdx).mData[1];
			out.z = norm->GetDirectArray().GetAt(vtxIdx).mData[2];
			break;
		}
		break;
	}
}

void MeshLoader::FBXGetUV(FbxMesh* in, int ctrlPointIdx, int vtxIdx, DirectX::XMFLOAT2A& out)
{
	FbxGeometryElementUV* uv = in->GetElementUV(0);
	switch (uv->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (uv->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
		{
			int idx = uv->GetIndexArray().GetAt(ctrlPointIdx);
			out.x = uv->GetDirectArray().GetAt(idx).mData[0];
			out.y = uv->GetDirectArray().GetAt(idx).mData[1];
			break;
		}
		case FbxGeometryElement::eDirect:
			out.x = uv->GetDirectArray().GetAt(ctrlPointIdx).mData[0];
			out.y = uv->GetDirectArray().GetAt(ctrlPointIdx).mData[1];
			break;
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (uv->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
		{
			int idx = uv->GetIndexArray().GetAt(vtxIdx);
			out.x = uv->GetDirectArray().GetAt(idx).mData[0];
			out.y = uv->GetDirectArray().GetAt(idx).mData[1];
			break;
		}
		case FbxGeometryElement::eDirect:
			out.x = uv->GetDirectArray().GetAt(vtxIdx).mData[0];
			out.y = uv->GetDirectArray().GetAt(vtxIdx).mData[1];
			break;
		}
		break;
	}
}

FbxMesh* getMesh(FbxNode* root)
{
	FbxNodeAttribute* attr = root->GetNodeAttribute();
	if (attr)
	{
		if (attr->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh* m = root->GetMesh();
			if (m) return m;
		}
	}
	int childCnt = root->GetChildCount();
	for (int i = 0; i < childCnt; ++i)
	{
		FbxMesh* val = getMesh(root->GetChild(i));
		if (val != nullptr) return val;
	}

	return nullptr;

}

MeshLoader::MeshLoader()
{
	if (instance != nullptr) return;

	instance = this;
}

void MeshLoader::LoadFBX(MeshData* out, std::string filename)
{
	FbxManager* fbx = FbxManager::Create();
	FbxIOSettings* ios = FbxIOSettings::Create(fbx, IOSROOT);
	fbx->SetIOSettings(ios);
	FbxImporter* fbxImporter = FbxImporter::Create(fbx, "");

	bool success = fbxImporter->Initialize(filename.c_str(), -1, fbx->GetIOSettings());

	if (!success) return;

	FbxScene* fbxScene = FbxScene::Create(fbx, "scene");
	fbxImporter->Import(fbxScene);
	fbxImporter->Destroy();

	FbxNode* rootNode = fbxScene->GetRootNode();

	FbxMesh* mesh = getMesh(rootNode);

	unsigned polygonCount = mesh->GetPolygonCount();
	out->indices.reserve(polygonCount * 3);
	out->vertices.reserve(polygonCount * 3);

	Vertex buff;
	int vtxIdx = 0;
	int vtxTot = 0;
	for (int i = 0; i < polygonCount; ++i)
	{
		int vcnt = mesh->GetPolygonSize(i);
		for (int j = 0; j < vcnt; ++j)
		{
			int ctrlPointIdx = mesh->GetPolygonVertex(i, j);
			FBXGetVertexPos(mesh, ctrlPointIdx, buff.pos);
			FBXGetNormal(mesh, ctrlPointIdx, vtxIdx, buff.normal);
			FBXGetUV(mesh, ctrlPointIdx, vtxIdx, buff.tex);
			out->vertices.push_back(buff);

			++vtxIdx;
		}

		for (int j = 1; j < vcnt - 1; ++j)
		{
			out->indices.push_back(vtxTot);
			out->indices.push_back(vtxTot + j);
			out->indices.push_back(vtxTot + j + 1);
		}

		vtxTot += vcnt;
	}
}

void MeshLoader::LoadOBJ(MeshData* mesh, std::string filename)
{
	
}
