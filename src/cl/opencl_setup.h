#ifndef OPENCL_SETUP_H
#define OPENCL_SETUP_H

#define CL_TARGET_OPENCL_VERSION 220
//#include <CL/cl.h> Hola Tomas esta pa los que no son apple
#include <OpenCL/cl.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

cl_context createContext();
cl_command_queue createCommandQueue(cl_context context, cl_device_id *device);
cl_program createProgram(cl_context context, cl_device_id device, const char* fileName);
void checkError(cl_int err, const char* operation);

#endif // OPENCL_SETUP_H