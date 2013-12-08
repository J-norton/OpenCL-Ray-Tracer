#pragma once
#include <GL/glew.h>
#include <CL/cl.hpp>
#include <gl/GL.h>
#include <GL/glut.h>
#include <CL/cl_gl.h>

class CLGLBuffer
{
public:
	//Code to initialize glut and create window
	bool initGlut(int argc, char** argv, int width, int height, void (_cdecl *mainLoop)());
	GLuint buffer_id;
	virtual void draw();
	CLGLBuffer(void);
	~CLGLBuffer(void);
};

