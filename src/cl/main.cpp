#include <chrono>
#include <fstream>
#include <iostream>

#include "kernel.h"

struct Times {
    long create_data;
    long copy_to_host;
    long execution;
    long copy_to_device;

    inline long total() const {
        return create_data + copy_to_host + execution + copy_to_device;
    }
};

Times t;
GpuLife gpuLife;

bool simulate(int N, int M, int localWorkSize, int iterations, int debug, int if_use) {
    using std::chrono::microseconds;
    gpuLife.resize(N, M);
    if (!gpuLife.allocBuffers()) {
        std::cerr << "Error while allocating buffers" << std::endl;
        return false;
    }

    uint worldSize = N * M;
    auto data = new ubyte[worldSize];
    auto t_start = std::chrono::high_resolution_clock::now();
    gpuLife.initRandom(data, worldSize);
    auto t_end = std::chrono::high_resolution_clock::now();
    t.create_data =
            std::chrono::duration_cast<microseconds>(t_end - t_start).count();

    // Copy values from host variables to device
    t_start = std::chrono::high_resolution_clock::now();
    gpuLife.copyToDevice(data, worldSize);
    t_end = std::chrono::high_resolution_clock::now();
    t.copy_to_device =
            std::chrono::duration_cast<microseconds>(t_end - t_start).count();

    t_start = std::chrono::high_resolution_clock::now();
    gpuLife.iterate(iterations, localWorkSize, debug, if_use);
    t_end = std::chrono::high_resolution_clock::now();
    t.execution =
            std::chrono::duration_cast<microseconds>(t_end - t_start)
                    .count();

    // Print the result
    std::cout << "RESULTS: " << std::endl;
    //std::cout << gpuLife;

    std::cout << "Time to create data: " << t.create_data << " microseconds\n";
    std::cout << "Time to execute kernel: " << t.execution << " microseconds\n";
    std::cout << "Time to execute the whole program: " << t.total()
              << " microseconds\n";

    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 8) {
        std::cerr << "Uso: " << argv[0]
                  << " <world width> <world height> <block size> <iterations> <output file> <0|1 (debug)> <0|1 (if)>"
                  << std::endl;
        return 2;
    }

    int n = std::stoi(argv[1]);
    int m = std::stoi(argv[2]);
    int bs = std::stoi(argv[3]);
    int it = std::stoi(argv[4]);
    auto file = argv[5];
    int db = std::stoi(argv[6]);
    int if_use = std::stoi(argv[7]);

    if (!simulate(n, m, bs, it, db, if_use)) {
        std::cerr << "Error while executing the simulation" << std::endl;
        return 3;
    }

    std::ofstream out;
    out.open(file, std::ios::app | std::ios::out);
    if (!out.is_open()) {
        std::cerr << "Error while opening file: '" << argv[2] << "'" << std::endl;
        return 4;
    }
    // params
    out << n * m << "," << bs << "," << if_use << "," << it << ",";
    // times
    out << t.create_data << "," << t.copy_to_device << "," << t.execution << "," << t.copy_to_host << "," << t.total()
        << "\n";

    std::cout << "Data written to " << file << std::endl;
    return 0;
}