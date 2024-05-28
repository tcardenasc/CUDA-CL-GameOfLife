#include <random>
#include <iostream>

typedef unsigned char ubyte;

class CpuLife {
private:
    static inline ubyte countAliveCells(const ubyte *data, size_t x0, size_t x1, size_t x2, size_t y0, size_t y1, size_t y2) {
        return data[x0 + y0] + data[x1 + y0] + data[x2 + y0]
               + data[x0 + y1] + data[x2 + y1]
               + data[x0 + y2] + data[x1 + y2] + data[x2 + y2];
    }
    static inline ubyte countAliveCellsIf(const ubyte *data, size_t x0, size_t x1, size_t x2, size_t y0, size_t y1, size_t y2) {
        size_t xOffsets[3] = {x0, x1, x2};
        size_t yOffsets[3] = {y0, y1, y2};

        ubyte aliveCells = 0;

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (i == 1 && j == 1) {
                    continue;
                }
                if (data[xOffsets[i] + yOffsets[j]]) {
                    aliveCells++;
                }
            }
        }
        return aliveCells;
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
    void iterateSerial(size_t iterations, int debug, int if_use);
    friend std::ostream &operator<<(std::ostream &os, const CpuLife &cpuLife);
};
