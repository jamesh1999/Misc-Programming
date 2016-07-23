#include "InitGLUT.h"

using namespace Core;


Core::IListener* Init_GLUT::listener;
Core::WindowInfo Init_GLUT::windowInformation;


void Init_GLUT::init(const Core::WindowInfo& windowInfo, const Core::ContextInfo& contextInfo, const Core::FramebufferInfo& framebufferInfo)
{
	//Fake arguments required for glutInit
	int fakeargc = 1;
	char *fakeargv[] = { "fake", NULL };
	glutInit(&fakeargc, fakeargv);

	if (contextInfo.core)
	{
		glutInitContextVersion(contextInfo.major_version, contextInfo.minor_version);
		glutInitContextProfile(GLUT_CORE_PROFILE);
	}
	else
	{
		glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	}

	glutInitDisplayMode(framebufferInfo.flags);
	glutInitWindowPosition(windowInfo.position_x, windowInfo.position_y);
	glutInitWindowSize(windowInfo.width, windowInfo.height);

	glutCreateWindow(windowInfo.name.c_str());

	std::cout << "GLUT : Initialized" << std::endl;
	
	//Callbacks
	glutIdleFunc(idleCallback);
	glutCloseFunc(closeCallback);
	glutDisplayFunc(displayCallback);
	glutReshapeFunc(reshapeCallback);

	initGLEW();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS); //Clean up window

	printOpenGLInfo(windowInfo, contextInfo);

	windowInformation = windowInfo;
}

//Starts the rendering loop
void Init_GLUT::run()
{
	std::cout << "GLUT : Start Running" << std::endl;
	glutMainLoop();
}

void Init_GLUT::close()
{
	std::cout << "GLUT : Finished" << std::endl;
	glutLeaveMainLoop();
}

void Init_GLUT::idleCallback(void)
{
	glutPostRedisplay(); //Do nothing
}

void Init_GLUT::displayCallback()
{
	if (listener) //NULL check
	{
		listener->notifyBeginFrame();
		listener->notifyDisplayFrame();

		glutSwapBuffers();

		listener->notifyEndFrame();
	}
}

void Init_GLUT::reshapeCallback(int width, int height)
{
	if (windowInformation.is_reshapable == true)
	{
		if (listener) //NULL check
		{
			listener->notifyReshape(width, height, windowInformation.width, windowInformation.height);
		}

		windowInformation.width = width;
		windowInformation.height = height;
	}
}

void Init_GLUT::closeCallback()
{
	close();
}

void Init_GLUT::enterFullscreen()
{
	glutFullScreen();
}

void Init_GLUT::exitFullscreen()
{
	glutLeaveFullScreen();
}

void Init_GLUT::printOpenGLInfo(const Core::WindowInfo& windowInfo, const Core::ContextInfo& contextInfo)
{
	const unsigned char* renderer = glGetString(GL_RENDERER);
	const unsigned char* vendor = glGetString(GL_VENDOR);
	const unsigned char* version = glGetString(GL_VERSION);

	std::cout << "*******************************************************************************" << std::endl;
	std::cout << "GLUT : Initialize" << std::endl;
	std::cout << "GLUT : Vendor - " << vendor << std::endl;
	std::cout << "GLUT : Renderer - " << renderer << std::endl;
	std::cout << "GLUT : OpenGl version - " << version << std::endl;
	std::cout << "*******************************************************************************" << std::endl;
}

void Init_GLUT::setListener(Core::IListener*& iListener)
{
	listener = iListener;
}