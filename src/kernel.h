#include <random>

typedef unsigned char ubyte;

class CpuLife {
protected:
    // Naming convention: m_ for member variables
    // World data
    ubyte *m_data;
    // Helper buffer
    ubyte *m_resultData;

    // World size
    size_t m_worldWidth;
    size_t m_worldHeight;
    size_t m_dataLength; // m_worldWidth * m_worldHeight

    // Random number generator
    std::mt19937 m_randomGen;

public:
    CpuLife();
    ~CpuLife();
};
