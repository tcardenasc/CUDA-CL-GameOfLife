#include <random>
#include <iostream>

typedef unsigned char ubyte;

class CpuLife {
private:
    static inline ubyte countAliveCells(ubyte *data, size_t x0, size_t x1, size_t x2, size_t y0, size_t y1, size_t y2) {
        return data[x0 + y0] + data[x1 + y0] + data[x2 + y0]
               + data[x0 + y1] + data[x2 + y1]
               + data[x0 + y2] + data[x1 + y2] + data[x2 + y2];
    }

protected:
    // Naming convention: m_ for member variables
    // World data
    ubyte *m_data;
    // Helper buffer
    ubyte *m_resultData;

    // World size
    size_t m_worldWidth;
    size_t m_worldHeight;
    size_t m_worldSize; // m_worldWidth * m_worldHeight

    // Random number generator
    std::mt19937 m_randomGen;

public:
    CpuLife();
    ~CpuLife();
    void freeBuffers();
    bool allocBuffers();
    void resize(size_t width, size_t height);
    void initRandom();
    void iterateSerial(size_t iterations);
    friend std::ostream &operator<<(std::ostream &os, const CpuLife &cpuLife);
};
