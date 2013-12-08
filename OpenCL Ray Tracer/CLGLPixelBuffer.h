#pragma once
#include "CLGLBuffer.h"
#include <iostream>


class CLGLPixelBuffer : public CLGLBuffer
{
public:
	GLuint renderbuff_id;
	float *raw_image;
	int height;
	int width;
	virtual void draw();
	void initTexture();
	void createBuffer();
	void exampleBuffer(GLint vbolen);
	CLGLPixelBuffer(int height, int width);
	CLGLPixelBuffer(void);
	CLGLPixelBuffer(const CLGLPixelBuffer& copy);
	CLGLPixelBuffer& operator=(const CLGLPixelBuffer& assign);
	~CLGLPixelBuffer(void);
};

