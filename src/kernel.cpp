#include "kernel.h"

CpuLife::CpuLife() :
        m_data(nullptr),
        m_resultData(nullptr),
        m_worldWidth(0),
        m_worldHeight(0),
        m_dataLength(0),
        m_randomGen(std::random_device()()) {}

CpuLife::~CpuLife() {
    delete[] m_data;
    m_data = nullptr;
    delete[] m_resultData;
    m_resultData = nullptr;
    m_dataLength = 0;
}


