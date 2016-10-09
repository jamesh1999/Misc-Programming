#include "Renderer.h"
#include "GraphicsController.h"

void Renderer::Render()
{
	for (int i = 0; i < mat->passes.size(); ++i)
	{
		GraphicsController::instance->devContext->IASetInputLayout(mat->passes[i].layout);
		GraphicsController::instance->devContext->VSSetShader(mat->passes[i].vs, NULL, NULL);
		GraphicsController::instance->devContext->PSSetShader(mat->passes[i].ps, NULL, NULL);

		GraphicsController::instance->devContext->DrawIndexed(mesh->indices.size(), 0, 0);
	}
}

Renderer::Renderer(Material* mat, MeshData* mesh)
	: mat(mat), mesh(mesh) {}

Renderer::~Renderer()
{
	delete mat;
	delete mesh;
}

void Renderer::SetMaterial(Material* m)
{
	delete mat;
	mat = m;
}

void Renderer::SetMesh(MeshData* m)
{
	delete mesh;
	mesh = m;
}