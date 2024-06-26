#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include "kernel.h"

struct Times {
    long create_data;
    long execution;

    long total() const { return create_data + execution; }
};

Times t;
CpuLife cpuLife;

bool simulate(int N, int M, int iterations, int debug, int if_use) {
    using std::chrono::microseconds;
    cpuLife.resize(N, M);
    if (!cpuLife.allocBuffers()) {
        std::cerr << "Error while allocating buffers" << std::endl;
        return false;
    }

    auto t_start = std::chrono::high_resolution_clock::now();
    cpuLife.initRandom();
    auto t_end = std::chrono::high_resolution_clock::now();
    t.create_data =
            std::chrono::duration_cast<microseconds>(t_end - t_start).count();

    t_start = std::chrono::high_resolution_clock::now();
    cpuLife.iterateSerial(iterations, debug, if_use);
    t_end = std::chrono::high_resolution_clock::now();
    t.execution =
            std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start)
                    .count();

    // Print the result
    std::cout << "RESULTS: " << std::endl;
//    std::cout << cpuLife;

    std::cout << "Time to create data: " << t.create_data << " microseconds\n";
    std::cout << "Time to execute kernel: " << t.execution << " microseconds\n";
    std::cout << "Time to execute the whole program: " << t.total()
              << " microseconds\n";

    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        std::cerr << "Uso: " << argv[0] << " <world width> <world height> <iterations> <output_file> <0|1 (debug)> <0|1 (if)>"
                  << std::endl;
        return 2;
    }

    int n = std::stoi(argv[1]), m = std::stoi(argv[2]), iterations = std::stoi(argv[3]), debug = std::stoi(argv[5]), if_use = std::stoi(argv[6]);
    if (!simulate(n, m, iterations, debug, if_use)) {
        std::cerr << "Error while executing the simulation" << std::endl;
        return 3;
    }

    std::ofstream out;
    out.open(argv[4], std::ios::app | std::ios::out);
    if (!out.is_open()) {
        std::cerr << "Error while opening file: '" << argv[4] << "'" << std::endl;
        return 4;
    }
    out << n << "," << m << "," << iterations << "," << t.create_data << "," << t.execution << "," << t.total()
        << "\n";

    std::cout << "Data written to " << argv[4] << std::endl;
    return 0;
}
