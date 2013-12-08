#include "CLGLBuffer.h"


CLGLBuffer::CLGLBuffer(void) :
	buffer_id(0)
{
}

bool CLGLBuffer::initGlut(int argc, char** argv, int width,
						  int height, void (_cdecl *mainLoop)() )
{
	//Initialize OpenGL stuff
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(150, 150);
	glutInitWindowSize(width, height);
	glutCreateWindow("Parallelized Ray Tracing");
	glutDisplayFunc(mainLoop);
	glewInit();

	if (glGetError() == GL_NO_ERROR)
		return true;
	else
		return false;
}


CLGLBuffer::~CLGLBuffer(void)
{
}

void CLGLBuffer::draw()
{
}
