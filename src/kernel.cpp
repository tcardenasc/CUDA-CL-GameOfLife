#include "kernel.h"

void Clear() {
#if defined _WIN32
    system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined (__APPLE__)
    system("clear");
#endif
}

CpuLife::CpuLife() :
        m_data(nullptr),
        m_resultData(nullptr),
        m_worldWidth(0),
        m_worldHeight(0),
        m_dataLength(0),
        m_randomGen(std::random_device()()) {}

CpuLife::~CpuLife() {
    freeBuffers();
}

void CpuLife::freeBuffers() {
    delete[] m_data;
    m_data = nullptr;
    delete[] m_resultData;
    m_resultData = nullptr;
    m_dataLength = 0;
}

bool CpuLife::allocBuffers() {
    freeBuffers();
    m_dataLength = m_worldWidth * m_worldHeight;

    try {
        m_data = new ubyte[m_dataLength];
        m_resultData = new ubyte[m_dataLength];
    }
    catch (std::bad_alloc &e) {
        freeBuffers();
        return false;
    }
    return true;
}

void CpuLife::resize(size_t width, size_t height) {
    freeBuffers();
    m_worldWidth = width;
    m_worldHeight = height;
}

void CpuLife::initRandom() {
    for (size_t i = 0; i < m_dataLength; ++i) {
        m_data[i] = m_randomGen() & 1;
    }
}

void CpuLife::iterateSerial(size_t iterations) {
    for (size_t i = 0; i < iterations; ++i) {
        for (size_t y = 0; y < m_worldHeight; ++y) {
            size_t y0 = ((y + m_worldHeight - 1) % m_worldHeight) * m_worldWidth;
            size_t y1 = y * m_worldWidth;
            size_t y2 = ((y + 1) % m_worldHeight) * m_worldWidth;

            for (size_t x = 0; x < m_worldWidth; ++x) {
                size_t x0 = (x + m_worldWidth - 1) % m_worldWidth;
                size_t x2 = (x + 1) % m_worldWidth;

                ubyte aliveCells = countAliveCells(m_data, x0, x, x2, y0, y1, y2);
                m_resultData[y1 + x] = aliveCells == 3 || (aliveCells == 2 && m_data[x + y1]) ? 1 : 0;
            }
        }

        std::swap(m_data, m_resultData);

        Clear();
        std::cout << *this;
    }
}

std::ostream &operator<<(std::ostream &os, const CpuLife &cpuLife) {
    for (size_t i = 0; i < cpuLife.m_dataLength; ++i) {
        os << (cpuLife.m_data[i] ? "■" : "□");
        if ((i + 1) % cpuLife.m_worldWidth == 0) {
            os << std::endl;
        }
    }
    return os;
}



