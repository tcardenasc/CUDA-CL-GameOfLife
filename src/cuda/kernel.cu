#include "kernel.cuh"

void Clear() {
#if defined _WIN32
    system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined (__APPLE__)
    system("clear");
#endif
}

__global__ void lifeKernel(const ubyte *lifeData, uint worldWidth, uint worldHeight, ubyte *resultLifeData) {
    uint worldSize = worldWidth * worldHeight;

    for (uint cellId = blockIdx.x * blockDim.x + threadIdx.x;
         cellId < worldSize;
         cellId += blockDim.x * gridDim.x) {

        uint x = cellId % worldWidth;
        uint yAbs = cellId - x;

        uint xLeft = (x + worldWidth - 1) % worldWidth;
        uint xRight = (x + 1) % worldWidth;

        uint yAbsUp = (yAbs + worldSize - worldWidth) % worldSize;
        uint yAbsDown = (yAbs + worldWidth) % worldSize;

        // Count alive cells.
        uint aliveCells = lifeData[xLeft + yAbsUp] + lifeData[x + yAbsUp] + lifeData[xRight + yAbsUp]
                          + lifeData[xLeft + yAbs] + lifeData[xRight + yAbs]
                          + lifeData[xLeft + yAbsDown] + lifeData[x + yAbsDown] + lifeData[xRight + yAbsDown];

        resultLifeData[x + yAbs] = aliveCells == 3 || (aliveCells == 2 && lifeData[x + yAbs]) ? 1 : 0;
    }
}

GpuLife::GpuLife() :
        d_data(nullptr),
        d_resultData(nullptr),
        m_worldWidth(0),
        m_worldHeight(0),
        m_worldSize(0),
        m_randomGen(std::random_device{}()) {
}

GpuLife::~GpuLife() {
    freeBuffers();
}

void GpuLife::freeBuffers() {
    HANDLE_ERROR(cudaFree(d_data));
    d_data = nullptr;

    HANDLE_ERROR(cudaFree(d_resultData));
    d_resultData = nullptr;
}

bool GpuLife::allocBuffers() {
    freeBuffers();
    m_worldSize = m_worldWidth * m_worldHeight;

    HANDLE_ERROR(cudaMalloc(&d_data, m_worldSize));
    HANDLE_ERROR(cudaMalloc(&d_resultData, m_worldSize));

    return true;
}

void GpuLife::resize(size_t width, size_t height) {
    freeBuffers();
    m_worldWidth = width;
    m_worldHeight = height;
}

void GpuLife::initRandom(ubyte *data, size_t size) {
    if (size != m_worldSize) {
        throw std::runtime_error("Size mismatch");
    }
    for (size_t i = 0; i < size; i++) {
        data[i] = m_randomGen() & 1;
    }
}

void GpuLife::copyToDevice(ubyte *data, size_t size) {
    if (size != m_worldSize) {
        throw std::runtime_error("Size mismatch");
    }
    HANDLE_ERROR(cudaMemcpy(d_data, data, size, cudaMemcpyHostToDevice));
}

void GpuLife::iterate(size_t iterations, size_t blockSize, int debug) {
    size_t gridSize = (m_worldWidth * m_worldHeight + blockSize - 1) / blockSize;

    for (size_t i = 0; i < iterations; i++) {
        lifeKernel<<<gridSize, blockSize>>>(d_data, m_worldWidth, m_worldHeight, d_resultData);
        HANDLE_ERROR(cudaGetLastError());
        HANDLE_ERROR(cudaDeviceSynchronize());
        std::swap(d_data, d_resultData);

        if (debug) {
            auto tmp = new ubyte[m_worldSize];
            HANDLE_ERROR(cudaMemcpy(tmp, d_resultData, m_worldSize, cudaMemcpyDeviceToHost));
            Clear();
            std::cout << "Iteration: " << i + 1 << std::endl;
            for (size_t k = 0; k < m_worldSize; k++) {
                std::cout << (tmp[k] ? "■" : "□");
                if ((k + 1) % m_worldWidth == 0) {
                    std::cout << std::endl;
                }
            }
        }
    }
}

void GpuLife::copyToHost(ubyte *data, size_t size) {
    if (size != m_worldSize) {
        throw std::runtime_error("Size mismatch");
    }
    HANDLE_ERROR(cudaMemcpy(data, d_data, size, cudaMemcpyDeviceToHost));
}
