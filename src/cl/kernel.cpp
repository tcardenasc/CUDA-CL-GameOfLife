// kernel.cpp
#include "kernel.h"
#include <vector>
#include <stdexcept>

void Clear() {
#if defined _WIN32
    system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined (__APPLE__)
    system("clear");
#endif
}

GpuLife::GpuLife() :
        m_worldWidth(0),
        m_worldHeight(0),
        m_worldSize(0),
        m_randomGen(std::random_device()()) { // Correct initialization
    context = createContext();
    commandQueue = createCommandQueue(context, &device);
    program = createProgram(context, device, "kernel.cl");
    kernel = clCreateKernel(program, "game_of_life", nullptr);
}

GpuLife::~GpuLife() {
    freeBuffers();
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(commandQueue);
    clReleaseContext(context);
}

void GpuLife::freeBuffers() {
    if (dataBuffer) clReleaseMemObject(dataBuffer);
    if (resultBuffer) clReleaseMemObject(resultBuffer);
    m_worldSize = 0;
}

bool GpuLife::allocBuffers() {
    freeBuffers();
    m_worldSize = m_worldWidth * m_worldHeight;

    cl_int err;
    dataBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(ubyte) * m_worldSize, nullptr, &err);
    checkError(err, "Creating data buffer");
    resultBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(ubyte) * m_worldSize, nullptr, &err);
    checkError(err, "Creating result buffer");

    return true;
}

void GpuLife::resize(size_t width, size_t height) {
    freeBuffers();
    m_worldWidth = width;
    m_worldHeight = height;
}

void GpuLife::initRandom() {
    std::vector<ubyte> data(m_worldSize);
    for (size_t i = 0; i < m_worldSize; ++i) {
        data[i] = m_randomGen() & 1;
    }

    cl_int err = clEnqueueWriteBuffer(commandQueue, dataBuffer, CL_TRUE, 0, sizeof(ubyte) * m_worldSize, data.data(), 0,
                                      nullptr, nullptr);
    checkError(err, "Writing initial data to buffer");
}

void GpuLife::iterate(size_t iterations, size_t workSize, int debug, int if_use) {
    cl_int err;

    size_t localWorkSize[] = {workSize};
    size_t globalWorkSize[] = {(m_worldSize + workSize - 1) / workSize * workSize};

    for (size_t i = 0; i < iterations; ++i) {
        err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &dataBuffer);
        checkError(err, "Setting kernel arg 0");
        err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &resultBuffer);
        checkError(err, "Setting kernel arg 1");
        err = clSetKernelArg(kernel, 2, sizeof(unsigned int), &m_worldWidth);
        checkError(err, "Setting kernel arg 2");
        err = clSetKernelArg(kernel, 3, sizeof(unsigned int), &m_worldHeight);
        checkError(err, "Setting kernel arg 3");

        err = clEnqueueNDRangeKernel(commandQueue, kernel, 1, nullptr, globalWorkSize, localWorkSize, 0, nullptr,
                                     nullptr);
        checkError(err, "Enqueueing kernel");
        clFinish(commandQueue);

        // Swap the data buffers
        std::swap(dataBuffer, resultBuffer);

        // Debug output to print the current iteration
        if (debug) {
            Clear();
            std::cout << "Iteration: " << i + 1 << std::endl;
            std::cout << *this;  // Print the current state of the world
        }
    }

    // Final state read from the buffer
    std::vector<ubyte> result(m_worldSize);
    err = clEnqueueReadBuffer(commandQueue, dataBuffer, CL_TRUE, 0, sizeof(ubyte) * m_worldSize, result.data(), 0,
                              nullptr, nullptr);
    checkError(err, "Reading result buffer");
}

std::ostream &operator<<(std::ostream &os, GpuLife &gpuLife) {
    std::vector<ubyte> data(gpuLife.m_worldSize);
    clEnqueueReadBuffer(gpuLife.commandQueue, gpuLife.dataBuffer, CL_TRUE, 0, sizeof(ubyte) * gpuLife.m_worldSize,
                        data.data(), 0, nullptr, nullptr);
    for (size_t i = 0; i < gpuLife.m_worldSize; ++i) {
        os << (data[i] ? "■" : "□");
        if ((i + 1) % gpuLife.m_worldWidth == 0) {
            os << std::endl;
        }
    }
    return os;
}