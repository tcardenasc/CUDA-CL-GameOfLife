#include <random>
#include <iostream>
#include <driver_types.h>

#define HANDLE_ERROR(_exp)                                             \
    do                                                                 \
    {                                                                  \
        const cudaError_t err = (_exp);                                \
        if (err != cudaSuccess)                                        \
        {                                                              \
            std::cerr << cudaGetErrorString(err) << " in " << __FILE__ \
                      << " at line " << __LINE__ << std::endl;         \
            exit(EXIT_FAILURE);                                        \
        }                                                              \
    } while (0)

typedef unsigned char ubyte;

__global__ void lifeKernel(const ubyte *lifeData, uint worldWidth, uint worldHeight, ubyte *resultLifeData);

class GpuLife {
protected:
    ubyte* d_data;
    ubyte* d_resultData;

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
    void iterate(size_t iterations, size_t blockSize, size_t gridSize);
    void copyToHost(ubyte *data, size_t size);
};