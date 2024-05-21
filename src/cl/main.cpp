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
#include <vector>
#include "kernel.h" // Include the header for the GpuLife class

struct Times {
  long create_data;
  long copy_to_host;
  long execution;
  long copy_to_device;
  inline long total() {
    return create_data + copy_to_host + execution + copy_to_device;
  }
};

Times t;
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
    std::cerr << "No GPU device found" << std::endl;
    return false;
  }

  std::cout << "GPU Used: " << devices.front().getInfo<CL_DEVICE_NAME>() << std::endl;

  cl::Context context(devices.front());
  queue = cl::CommandQueue(context, devices.front());

  std::ifstream sourceFile("kernel.cl");
  std::stringstream sourceCode;
  sourceCode << sourceFile.rdbuf();

  prog = cl::Program(context, sourceCode.str(), true);

  return true;
}

bool simulate(int N, int M, int iterations) {
  using std::chrono::microseconds;
  size_t size = N * M * sizeof(unsigned char);
  std::vector<unsigned char> data(N * M), result(N * M);

  // Create the memory buffers
  cl::Buffer dataBuffer(queue.getInfo<CL_QUEUE_CONTEXT>(), CL_MEM_READ_WRITE, size);
  cl::Buffer resultBuffer(queue.getInfo<CL_QUEUE_CONTEXT>(), CL_MEM_READ_WRITE, size);

  // Initialize random data
  auto t_start = std::chrono::high_resolution_clock::now();
  std::mt19937 randomGen(std::random_device{}());
  for (size_t i = 0; i < N * M; ++i) {
    data[i] = randomGen() & 1;
  }
  auto t_end = std::chrono::high_resolution_clock::now();
  t.create_data = std::chrono::duration_cast<microseconds>(t_end - t_start).count();

  // Copy values from host variables to device
  t_start = std::chrono::high_resolution_clock::now();
  queue.enqueueWriteBuffer(dataBuffer, CL_TRUE, 0, size, data.data());
  t_end = std::chrono::high_resolution_clock::now();
  t.copy_to_device = std::chrono::duration_cast<microseconds>(t_end - t_start).count();

  // Make kernel
  cl::Kernel kernel(prog, "game_of_life");

  // Set the kernel arguments
  kernel.setArg(0, dataBuffer);
  kernel.setArg(1, resultBuffer);
  kernel.setArg(2, static_cast<unsigned int>(N));
  kernel.setArg(3, static_cast<unsigned int>(M));

  // Execute the kernel on the device
  cl::NDRange globalSize(N, M);
  cl::NDRange localSize(16, 16);  // Assuming a 16x16 workgroup size

  t_start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; ++i) {
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, globalSize, localSize);
    queue.finish();
    std::swap(dataBuffer, resultBuffer);
    kernel.setArg(0, dataBuffer);
    kernel.setArg(1, resultBuffer);
  }
  t_end = std::chrono::high_resolution_clock::now();
  t.execution = std::chrono::duration_cast<microseconds>(t_end - t_start).count();

  // Copy the output variable from device to host
  t_start = std::chrono::high_resolution_clock::now();
  queue.enqueueReadBuffer(dataBuffer, CL_TRUE, 0, size, data.data());
  t_end = std::chrono::high_resolution_clock::now();
  t.copy_to_host = std::chrono::duration_cast<microseconds>(t_end - t_start).count();

  // Print the result
  std::cout << "RESULTS: " << std::endl;
// Uncomment the following lines to print the game board
  for (int i = 0; i < N * M; i++) {
    std::cout << (data[i] ? "■" : "□");
    if ((i + 1) % N == 0) {
      std::cout << std::endl;
    }
  }

  std::cout << "Time to create data: " << t.create_data << " microseconds\n";
  std::cout << "Time to copy data to device: " << t.copy_to_device << " microseconds\n";
  std::cout << "Time to execute kernel: " << t.execution << " microseconds\n";
  std::cout << "Time to copy data to host: " << t.copy_to_host << " microseconds\n";
  std::cout << "Time to execute the whole program: " << t.total() << " microseconds\n";

  return true;
}

int main(int argc, char* argv[]) {
  if (!init()) return 1;

  if (argc != 5) {
    std::cerr << "Usage: " << argv[0] << " <world width> <world height> <iterations> <output file>" << std::endl;
    return 2;
  }
  int n = std::stoi(argv[1]);
  int m = std::stoi(argv[2]);
  int iterations = std::stoi(argv[3]);

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

  out << n << "," << m << "," << iterations << "," << t.create_data << "," << t.copy_to_device << "," << t.execution << "," << t.copy_to_host << "," << t.total() << "\n";

  std::cout << "Data written to " << argv[4] << std::endl;
  return 0;
}