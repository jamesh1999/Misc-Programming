#include "Model.h"

using namespace Rendering;
using namespace Models;

Model::Model()
{

}

Model::~Model()
{
	Destroy();
}

void Model::Draw()
{
	
}

void Model::Update()
{
	
}

void Model::SetProgram(GLuint shaderName)
{
	program = shaderName;
}

GLuint Model::GetVao() const
{
	return vao;
}

const std::vector<GLuint>& Model::GetVbos() const
{
	return vbos;
}

void Model::Destroy()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(vbos.size(), &vbos[0]);
	vbos.clear();
}