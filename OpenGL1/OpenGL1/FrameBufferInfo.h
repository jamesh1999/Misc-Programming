#ifndef __FRAME_BUFFER_INFO_INCLUDED__
#define __FRAME_BUFFER_INFO_INCLUDED__

#include "GL/glew.h"
#include "GL/freeglut.h"

namespace Core
{

	struct FramebufferInfo
	{
		unsigned int flags;
		bool msaa;

		FramebufferInfo() : flags(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH), msaa(false) {};

		FramebufferInfo(bool color, bool depth, bool stencil, bool msaa) : msaa(msaa)
		{
			flags = GLUT_DOUBLE;

			if (color) flags |= GLUT_RGBA | GLUT_ALPHA;
			if (depth) flags |= GLUT_DEPTH;
			if (stencil) flags |= GLUT_STENCIL;
			if (msaa) flags |= GLUT_MULTISAMPLE;
		}

		FramebufferInfo(const FramebufferInfo& frame_buffer_info)
		{
			flags = frame_buffer_info.flags;
			msaa = frame_buffer_info.msaa;
		}

		void operator=(const FramebufferInfo& frame_buffer_info)
		{
			flags = frame_buffer_info.flags;
			msaa = frame_buffer_info.msaa;
		}
	};
}

#endif