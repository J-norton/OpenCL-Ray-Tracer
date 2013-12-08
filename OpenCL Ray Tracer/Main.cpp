//Defined to enable exception throwing
#ifndef __CL_ENABLE_EXCEPTIONS
#define __CL_ENABLE_EXCEPTIONS
#endif

#include "CLGLPixelBuffer.h"
#include <fstream>
#include <string>

struct GlobalEverything
{
	cl::CommandQueue queue;
	cl::Program program;
	cl::Context context;
	cl::Device device;
	cl::Kernel kernel;
	cl::Image2D image;
	cl::Buffer gl_image;
	cl::NDRange globalWorkSize;
	cl::NDRange localWorkSize;
	CLGLPixelBuffer frame_buffer;
	cl::BufferRenderGL gl_buff_obj;
};

//The framebuffer
unsigned int height = 512;
unsigned int width = 512;
unsigned char renderedImage[512 * 512 * 4];
int frame_number = 0;
struct GlobalEverything glob;

void reportError(cl::Error e)
{
	std::cout << "Error occurred calling the function: " << e.what() << std::endl;
	switch (e.err())
	{
	case CL_INVALID_CONTEXT:
		std::cout << "Invalid context" << std::endl;
		break;
	case CL_INVALID_MEM_OBJECT:
		std::cout << "Memory objects are invalid" << std::endl;
		break;
	case CL_INVALID_VALUE:
		std::cout << "Kernels are accessing unallocated memory" << std::endl;
		break;
	case CL_INVALID_EVENT_WAIT_LIST:
		std::cout << "Objects inside wait list are invalid" << std::endl;
		break;
	case CL_MISALIGNED_SUB_BUFFER_OFFSET:
		std::cout << "Sub buffer misaligned" << std::endl;
		break;
	case CL_MEM_COPY_OVERLAP:
		std::cout << "Destination buffer and source buffer are the same" << std::endl;
		break;
	case CL_INVALID_PROGRAM_EXECUTABLE:
		std::cout << "There is no executable located on the device with the command-queue" << std::endl;
		break;
	case CL_INVALID_KERNEL:
		std::cout << "Kernel is invalid" << std::endl;
		break;
	case CL_INVALID_KERNEL_ARGS:
		std::cout << "Kernel args are invalid" << std::endl;
		break;
	case CL_INVALID_GLOBAL_WORK_SIZE:
		std::cout << "Invalid global work size" << std::endl;
		break;
	case CL_INVALID_GLOBAL_OFFSET:
		std::cout << "Invalid global offset" << std::endl;
		break;
	case CL_INVALID_WORK_GROUP_SIZE:
		std::cout << "Invalid work group size" << std::endl;
		break;
	case CL_INVALID_WORK_ITEM_SIZE:
		std::cout << "Work item size is invalid" << std::endl;
		break;
	case CL_INVALID_IMAGE_SIZE:
		std::cout << "Invalid image size" << std::endl;
		break;
	case CL_OUT_OF_RESOURCES:
		std::cout << "No more resources on device" << std::endl;
		break;
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		std::cout << "Error allocating memory for object on device" << std::endl;
		break;
	case CL_OUT_OF_HOST_MEMORY:
		std::cout << "Congrats! You used all of the host's memory!" << std::endl;
		break;
	case CL_INVALID_IMAGE_DESCRIPTOR:
		std::cout << "Invalid image descriptor" << std::endl;
		break;
	case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
		std::cout << "Invalid image format descriptor" << std::endl;
		break;
	case CL_IMAGE_FORMAT_NOT_SUPPORTED:
		std::cout << "Image format not supported" << std::endl;
		break;
	default:
		std::cout << "Unknown error type" << std::endl;
	}

	std::string blah;
	std::cout << "Press any key to continue. . ." << std::endl;
	std::cin >> blah;
	std::exit(4);
}

void CL_CALLBACK contextCallback(const char* info, const void* userinfo, size_t cb, void* userdata)
{
	std::cout << "Errors bad" << std::endl;
}

void mainLoop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	try {
		glob.kernel.setArg<cl_int>(3, frame_number++);
		cl_int error = CL_SUCCESS;
		glFinish();
		error = clEnqueueAcquireGLObjects(glob.queue(), 1, &glob.image(), 0, nullptr, nullptr);

		glob.queue.enqueueNDRangeKernel(glob.kernel, cl::NullRange, glob.globalWorkSize, glob.localWorkSize, NULL, NULL);
		error = clEnqueueReleaseGLObjects(glob.queue(), 1, &glob.image(), 0, nullptr, nullptr);

		glob.queue.finish();
	}
	catch (cl::Error e) {
		reportError(e);
	}
	glob.frame_buffer.draw();
	glutPostRedisplay();
	glutSwapBuffers();
}

void reshapeLoop(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv)
{
	// List of all platforms and devices
	std::vector<cl::Event> event_queue;
	std::vector<cl::Platform> platforms;
	std::vector<cl::Device> devices;
	cl::Platform::get(&platforms);
	//Parameters to the kernel
	glob.localWorkSize = cl::NDRange(1, 1);
	glob.globalWorkSize = cl::NDRange(height, width);
	glob.frame_buffer = CLGLPixelBuffer(height, width);

	if ( !glob.frame_buffer.initGlut(argc, argv, width, height, mainLoop) )
		std::cout << "Something went wrong initializing glut" << std::endl;
	glutReshapeFunc(reshapeLoop);

	glob.frame_buffer.createBuffer();

	//Get the platforms, devices and contexts
	try {
		cl::Platform::get(&platforms);
		platforms[0].getDevices(CL_DEVICE_TYPE_ALL, &devices);
		//Set properties to allow OpenCL/OpenGL interop
		cl_context_properties properties[7] = { CL_GL_CONTEXT_KHR,
			(cl_context_properties) wglGetCurrentContext(), CL_WGL_HDC_KHR,
			(cl_context_properties) wglGetCurrentDC(), CL_CONTEXT_PLATFORM,
			(cl_context_properties) (platforms[0])(), 0 };
		glob.context = cl::Context(devices, properties, NULL, NULL);
	}
	catch (cl::Error e) {
		reportError(e);
	}

	//Now compile the kernels
	std::ifstream src_file("Test.cl");
	std::string src_prog(std::istreambuf_iterator<char>(src_file), (std::istreambuf_iterator<char>()) );
	const char* c_str_src = src_prog.c_str();
	cl::Program::Sources sources = cl::Program::Sources();
	sources.push_back(std::make_pair(c_str_src, (size_t) src_prog.length() ));

	try {
		glob.program = cl::Program(glob.context, sources);
		glob.program.build();
		glob.kernel = cl::Kernel(glob.program, "traceSphere");
		glob.queue = cl::CommandQueue(glob.context, devices[0]);

		cl::ImageFormat format;
		format.image_channel_data_type = CL_UNSIGNED_INT8;
		format.image_channel_order = CL_RGBA;

		cl_int err_temp = CL_SUCCESS;
		cl_mem renderbuff_obj = clCreateFromGLTexture(glob.context(),
			CL_MEM_WRITE_ONLY, GL_TEXTURE_RECTANGLE_ARB, 0, glob.frame_buffer.renderbuff_id, &err_temp);
		if (err_temp != CL_SUCCESS)
			std::cout << "Error creating CL image from GL texture";

		//glob.image = cl::ImageGL(glob.context, CL_MEM_WRITE_ONLY, GL_TEXTURE_RECTANGLE_ARB,
		//	0, glob.frame_buffer.renderbuff_id);
		glob.image = cl::Image2D(renderbuff_obj);

		glob.kernel.setArg<cl::Image2D>(0, glob.image);
		glob.kernel.setArg<cl_uint>(1, height);
		glob.kernel.setArg<cl_uint>(2, width);
		glob.kernel.setArg<cl_int>(3, frame_number);
	}
	catch (cl::Error e) {
		reportError(e);
	}

	glutMainLoop();
	return 0;
}
