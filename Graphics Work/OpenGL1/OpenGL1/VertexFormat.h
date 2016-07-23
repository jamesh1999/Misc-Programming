#ifndef  __VERTEX_FORMAT_INCLUDED__
#define __VERTEX_FORMAT_INCLUDED__

#include "glm\glm.hpp"

namespace Rendering
{

	struct VertexFormat
	{
		glm::vec3 position;
		glm::vec4 color;

		VertexFormat(const glm::vec3 &iPos, const glm::vec4 &iColor)
		{
			position = iPos;
			color = iColor;
		}

		VertexFormat() {}

	};

}

#endif