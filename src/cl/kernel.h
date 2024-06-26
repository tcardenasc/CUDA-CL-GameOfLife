
// kernel.h
#ifndef KERNEL_H
#define KERNEL_H

#include <random>
#include <iostream>
#include "opencl_setup.h"

typedef unsigned char ubyte;

class GpuLife {
private:
    cl_context context;
    cl_command_queue commandQueue;
    cl_program program;
    cl_kernel kernel;
    cl_device_id device{};

    cl_mem dataBuffer{};
    cl_mem resultBuffer{};

    size_t m_worldWidth;
    size_t m_worldHeight;
    size_t m_worldSize;

    std::mt19937 m_randomGen;

public:
    GpuLife();
    ~GpuLife();
    void freeBuffers();
    bool allocBuffers();
    void resize(size_t width, size_t height);
    void initRandom(ubyte *data, size_t size);
    void copyToDevice(ubyte *data, size_t size);
    void iterate(size_t iterations, size_t workSize, int debug, int if_use);
    friend std::ostream &operator<<(std::ostream &os, GpuLife &gpuLife);
};

#endif // KERNEL_H
