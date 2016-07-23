#ifndef __INIT_GLEW_INCLUDED__
#define __INIT_GLEW_INCLUDED__

#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"

namespace Core
{
	static void initGLEW()
	{
		glewExperimental = true;
		if (glewInit() == GLEW_OK)
		{
			std::cout << "GLEW: Initialize" << std::endl;
		}

		if (glewIsSupported("GL_VERSION_4_4"))
		{
			std::cout << "GLEW : GL_VERSION_4_4 is 4.4" << std::endl;
		}
		else
		{
			std::cout << " GLEW : GL_VERSION_4_4 not supported" << std::endl;
		}
	}
}

#endif