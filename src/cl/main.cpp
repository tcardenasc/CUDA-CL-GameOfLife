<<<<<<< Updated upstream

#include <cstddef>
#ifdef __APPLE__
#include <OpenCL/opencl.hpp>
#else
#include <CL/opencl.hpp>
#endif  // DEBUG
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
=======
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include "kernel.h"
>>>>>>> Stashed changes

struct Times {
    long create_data;
    long execution;

    long total() const { return create_data + execution; }
};

Times t;
<<<<<<< Updated upstream
cl::Program prog;
cl::CommandQueue queue;

bool init() {
  std::vector<cl::Platform> platforms;
  std::vector<cl::Device> devices;
  cl::Platform::get(&platforms);
  for (auto& p : platforms) {
    p.getDevices(CL_DEVICE_TYPE_GPU, &devices);
    if (devices.size() > 0) break;
  }
  if (devices.size() == 0) {
    std::cerr << "Not GPU device found" << std::endl;
    return false;
  }

  std::cout << "GPU Used: " << devices.front().getInfo<CL_DEVICE_NAME>()
            << std::endl;

  cl::Context context(devices.front());
  queue = cl::CommandQueue(context, devices.front());

  std::ifstream sourceFile("kernel.cl");
  std::stringstream sourceCode;
  sourceCode << sourceFile.rdbuf();

  prog = cl::Program(context, sourceCode.str(), true);

  return true;
}

bool simulate(int N, int localSize, int globalSize) {
  using std::chrono::microseconds;
  std::size_t size = sizeof(int) * N;
  std::vector<int> a(N), b(N), c(N);

  // Create the memory buffers
  cl::Buffer aBuff(queue.getInfo<CL_QUEUE_CONTEXT>(), CL_MEM_READ_WRITE, size);
  cl::Buffer bBuff(queue.getInfo<CL_QUEUE_CONTEXT>(), CL_MEM_READ_WRITE, size);
  cl::Buffer cBuff(queue.getInfo<CL_QUEUE_CONTEXT>(), CL_MEM_READ_WRITE, size);

  // Assign values to host variables
  auto t_start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N; i++) {
    a[i] = std::rand() % 2000;
    b[i] = std::rand() % 2000;
    c[i] = 0;
  }
  auto t_end = std::chrono::high_resolution_clock::now();
  t.create_data =
      std::chrono::duration_cast<microseconds>(t_end - t_start).count();

  // Copy values from host variables to device
  t_start = std::chrono::high_resolution_clock::now();
  // usar CL_FALSE para hacerlo asíncrono
  queue.enqueueWriteBuffer(aBuff, CL_TRUE, 0, size, a.data());
  queue.enqueueWriteBuffer(bBuff, CL_TRUE, 0, size, b.data());
  t_end = std::chrono::high_resolution_clock::now();
  t.copy_to_device =
      std::chrono::duration_cast<microseconds>(t_end - t_start).count();

  // Make kernel
  cl::Kernel kernel(prog, "vec_sum");

  // Set the kernel arguments
  kernel.setArg(0, aBuff);
  kernel.setArg(1, bBuff);
  kernel.setArg(2, cBuff);
  kernel.setArg(3, N);

  // Execute the function on the device (using 32 threads here)
  cl::NDRange gSize(globalSize);
  cl::NDRange lSize(localSize);

  t_start = std::chrono::high_resolution_clock::now();
  queue.enqueueNDRangeKernel(kernel, cl::NullRange, globalSize, localSize);
  queue.finish();
  t_end = std::chrono::high_resolution_clock::now();
  t.execution =
      std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start)
          .count();

  // Copy the output variable from device to host
  t_start = std::chrono::high_resolution_clock::now();
  queue.enqueueReadBuffer(cBuff, CL_TRUE, 0, size, c.data());
  t_end = std::chrono::high_resolution_clock::now();
  t.copy_to_host =
      std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start)
          .count();

  // Print the result
  std::cout << "RESULTS: " << std::endl;
  for (int i = 0; i < N; i++)
    std::cout << "  out[" << i << "]: " << c[i] << " (" << a[i] << " + " << b[i]
              << ")\n";

  std::cout << "Time to create data: " << t.create_data << " microseconds\n";
  std::cout << "Time to copy data to device: " << t.copy_to_device
            << " microseconds\n";
  std::cout << "Time to execute kernel: " << t.execution << " microseconds\n";
  std::cout << "Time to copy data to host: " << t.copy_to_host
            << " microseconds\n";
  std::cout << "Time to execute the whole program: " << t.total()
            << " microseconds\n";
  return true;
=======
GpuLife gpuLife;

bool simulate(int N, int M, int iterations) {
    using std::chrono::microseconds;
    gpuLife.resize(N, M);
    if (!gpuLife.allocBuffers()) {
        std::cerr << "Error while allocating buffers" << std::endl;
        return false;
    }

    auto t_start = std::chrono::high_resolution_clock::now();
    gpuLife.initRandom();
    auto t_end = std::chrono::high_resolution_clock::now();
    t.create_data =
            std::chrono::duration_cast<microseconds>(t_end - t_start).count();

    t_start = std::chrono::high_resolution_clock::now();
    gpuLife.iterate(iterations);
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
>>>>>>> Stashed changes
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        std::cerr << "Uso: " << argv[0] << " <world width> <world height> <iterations> <output_file>"
                  << std::endl;
        return 2;
    }

<<<<<<< Updated upstream
  if (argc != 5) {
    std::cerr << "Uso: " << argv[0]
              << " <array size> <local size> <global size> <output file>"
              << std::endl;
    return 2;
  }
  int n = std::stoi(argv[1]);
  int ls = std::stoi(argv[2]);
  int gs = std::stoi(argv[3]);

  if (!simulate(n, ls, gs)) {
    std::cerr << "CL: Error while executing the simulation" << std::endl;
    return 3;
  }

  std::ofstream out;
  out.open(argv[4], std::ios::app | std::ios::out);
  if (!out.is_open()) {
    std::cerr << "Error while opening file: '" << argv[2] << "'" << std::endl;
    return 4;
  }
  // params
  out << n << "," << ls << "," << gs << ",";
  // times
  out << t.create_data << "," << t.copy_to_device << "," << t.execution << ","
      << t.copy_to_host << "," << t.total() << "\n";

  std::cout << "Data written to " << argv[4] << std::endl;
  return 0;
}
=======
    int n = std::stoi(argv[1]), m = std::stoi(argv[2]), iterations = std::stoi(argv[3]);
    if (!simulate(n, m, iterations)) {
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
>>>>>>> Stashed changes
