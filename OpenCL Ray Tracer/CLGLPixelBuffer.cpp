#include "CLGLPixelBuffer.h"


CLGLPixelBuffer::CLGLPixelBuffer(void) :
	width(512),
	height(512)
{
	raw_image = new float[width * height * 3];
}

CLGLPixelBuffer::CLGLPixelBuffer(int height, int width) :
	renderbuff_id(0),
	height(height),
	width(width)
{
	buffer_id = 0;
	raw_image = new float[width * height * 3];
}

CLGLPixelBuffer::CLGLPixelBuffer(const CLGLPixelBuffer& copy)
{
	width = copy.width;
	height = copy.height;
	raw_image = new float[width * height * 3];
	for (int i = 0; i < width * height * 3; i++)
		raw_image[i] = copy.raw_image[i];
}

CLGLPixelBuffer& CLGLPixelBuffer::operator=(const CLGLPixelBuffer& assign)
{
	if (this == &assign)
		return *this;
	delete[] raw_image;
	width = assign.width;
	height = assign.height;
	raw_image = new float[width * height * 3];
	for (int i = 0; i < width * height * 3; i++)
		raw_image[i] = assign.raw_image[i];
}


CLGLPixelBuffer::~CLGLPixelBuffer(void)
{
	delete[] raw_image;
}

//void CLGLPixelBuffer::calculateImage(struct GlobalEverything* glob)
//{
//	cl::Buffer buff = cl::BufferGL(glob->gl_buffer_obj);
//	std::vector<cl::Memory> mem_obj = std::vector<cl::Memory>();
//	mem_obj.push_back(buff);
//	glob->kernel.setArg<cl_mem>(0, glob->gl_buffer_obj);
//	glob->kernel.setArg<cl_uint>(1, width);
//	glob->kernel.setArg<cl_uint>(2, height);
//
//	glFinish();
//	glob->queue.enqueueAcquireGLObjects(&mem_obj);
//	
//	glob->queue.enqueueNDRangeKernel(glob->kernel, cl::NullRange, glob->globalWorkSize, glob->localWorkSize);
//	glob->queue.finish();
//	glob->queue.enqueueReleaseGLObjects(&mem_obj);
//}

void CLGLPixelBuffer::initTexture()
{
	glGenTextures(1, &buffer_id);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, buffer_id);
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA32F_ARB, width, height, 0, GL_LUMINANCE, GL_FLOAT, NULL);
}

void CLGLPixelBuffer::exampleBuffer(GLint vbolen)
{
	glGenBuffers(1, &buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	glBufferData(GL_ARRAY_BUFFER, vbolen * sizeof(float), NULL, GL_STREAM_DRAW);

	//Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CLGLPixelBuffer::draw()
{
	GLenum err_code = GL_NO_ERROR;

	glEnable(GL_TEXTURE_RECTANGLE_ARB);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, renderbuff_id);
	glBegin(GL_QUADS);

	glTexCoord2i(0, 0);
	glVertex2i(0, 0);

	glTexCoord2i(0, height);
	glVertex2i(0, height);

	glTexCoord2i(width, height);
	glVertex2i(width, height);

	glTexCoord2i(width, 0);
	glVertex2i(width, 0);

	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_RECTANGLE_ARB);
}

void CLGLPixelBuffer::createBuffer()
{
	glClearColor(0, 1, 0, 1);
	//glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &renderbuff_id);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	if (glGetError() != GL_NO_ERROR)
		std::cout << "errorgl";
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, renderbuff_id);
	if (glGetError() != GL_NO_ERROR)
		std::cout << "errorgl";

	//glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//if (glGetError() != GL_NO_ERROR)
	//	std::cout << "errorgl";
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//if (glGetError() != GL_NO_ERROR)
	//	std::cout << "errorgl";
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//if (glGetError() != GL_NO_ERROR)
	//	std::cout << "errorgl";
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//if (glGetError() != GL_NO_ERROR)
	//	std::cout << "errorgl";

	//Byte-long color channel
	//glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA8, width, height,
	//	0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA32F_ARB, width, height,
		0, GL_LUMINANCE, GL_FLOAT, nullptr);
	if (glGetError() != GL_NO_ERROR)
		std::cout << "errorgl";

}