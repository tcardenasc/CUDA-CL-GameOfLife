#include "opencl_setup.h"

cl_context createContext() {
    cl_int err;
    cl_uint numPlatforms;
    cl_platform_id platformId = NULL;
    cl_context context = NULL;

    err = clGetPlatformIDs(1, &platformId, &numPlatforms);
    checkError(err, "Finding platforms");

    cl_context_properties contextProperties[] = {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)platformId,
        0
    };
    context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_GPU, NULL, NULL, &err);
    checkError(err, "Creating context");

    return context;
}

cl_command_queue createCommandQueue(cl_context context, cl_device_id *device) {
    cl_int err;
    size_t deviceBufferSize = -1;

    err = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &deviceBufferSize);
    checkError(err, "Getting device buffer size");

    if (deviceBufferSize <= 0) {
        std::cerr << "No devices available.\n";
        exit(1);
    }

    std::vector<cl_device_id> devices(deviceBufferSize / sizeof(cl_device_id));
    err = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceBufferSize, &devices[0], NULL);
    checkError(err, "Getting devices");

    *device = devices[0];

    cl_command_queue commandQueue = clCreateCommandQueue(context, *device, 0, &err);
    checkError(err, "Creating command queue");

    return commandQueue;
}

cl_program createProgram(cl_context context, cl_device_id device, const char* fileName) {
    cl_int err;
    std::ifstream kernelFile(fileName, std::ios::in);
    if (!kernelFile.is_open()) {
        std::cerr << "Failed to open file for reading: " << fileName << std::endl;
        exit(1);
    }

    std::ostringstream oss;
    oss << kernelFile.rdbuf();
    std::string srcStdStr = oss.str();
    const char *srcStr = srcStdStr.c_str();

    cl_program program = clCreateProgramWithSource(context, 1, (const char**)&srcStr, NULL, &err);
    checkError(err, "Creating program");

    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        size_t len;
        char buffer[2048];
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        std::cerr << "Build log:\n" << buffer << std::endl;
        checkError(err, "Building program");
    }

    return program;
}

void checkError(cl_int err, const char* operation) {
    if (err != CL_SUCCESS) {
        std::cerr << "Error during operation '" << operation << "': " << err << std::endl;
        exit(1);
    }
}