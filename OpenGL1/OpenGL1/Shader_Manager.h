#ifndef __SHADER_MANAGER_INCLUDED__
#define __SHADER_MANAGER_INCLUDED__

#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"

namespace Managers
{
 
 class Shader_Manager
 {
 public:
	Shader_Manager(void);
	~Shader_Manager(void);
	void CreateProgram(const std::string& shaderName,
                       const std::string& VertexShaderFilename,
                       const std::string& FragmentShaderFilename);
	static const GLuint GetShader(const std::string&);
	static const GLuint RemoveShader(const std::string&);

private:
	std::string ReadShader(const std::string& filename);
	GLuint CreateShader(GLenum shaderType,
                        const std::string& source,
                        const std::string& shaderName);
	static std::map<std::string, GLuint> programs;
 };

}

#endif