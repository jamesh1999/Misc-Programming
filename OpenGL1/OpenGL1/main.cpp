#include "InitGLUT.h"
#include "Scene_Manager.h"


int main(int argc, char **argv) {

	Core::WindowInfo window(std::string("OpenGL Window"), 400, 200, 800, 600, true);

	Core::ContextInfo context(4, 4, true);
	Core::FramebufferInfo frameBufferInfo(true, true, true, true);
	Core::Init_GLUT::init(window, context, frameBufferInfo);

	Core::IListener* scene = new Managers::Scene_Manager();
	Core::Init_GLUT::setListener(scene);

	Core::Init_GLUT::run();

	delete scene;

	return 0;
}