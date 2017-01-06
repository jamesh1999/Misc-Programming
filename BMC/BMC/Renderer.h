#ifndef __GRAPHICS_OBJECT_INCLUDED___
#define __GRAPHICS_OBJECT_INCLUDED___

#include <DirectXMath.h>
#include "Component.h"
#include "Material.h"
#include "MeshData.h"

class Renderer : public Component
{
	friend class GraphicsController;

private:
	Material* mat;
	MeshData* mesh;

public:


	void Render();

	Renderer(Material* mat, MeshData* mesh);
	~Renderer();

	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) = delete;

	void SetMesh(MeshData*);
	void SetMaterial(Material*);
};

#endif