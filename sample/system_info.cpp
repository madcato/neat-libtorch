#include <iostream>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <thread>
#include <vector>

#include <torch/torch.h>
#ifdef CUDA_LIB_AVAILABLE
#include "cuda_runtime_api.h"
#endif  // CUDA_LIB_AVAILABLE

int main() {
    std::cout << std::endl;

    std::cout << "PyTorch version: "
    << TORCH_VERSION_MAJOR << "."
    << TORCH_VERSION_MINOR << "."
    << TORCH_VERSION_PATCH << std::endl;

    std::cout << std::endl;

   torch::Device device = torch::kCPU;
    if (torch::cuda::is_available()) {
        std::cout << "CUDA is available! Training on GPU." << std::endl;
        device = torch::kCUDA;
#ifdef CUDA_LIB_AVAILABLE
        for(size_t i = 0 ; i < torch::cuda::device_count() ; i++) {
            cudaDeviceProp prop;
            if (cudaSuccess == cudaGetDeviceProperties(&prop, i)) {
                std::cout << "Device " << dev << " name: " << deviceProp.name << std::endl;
                std::cout << " Computational Capabilities: " << deviceProp.major << "." << deviceProp.minor << std::endl;
                std::cout << " Maximum global memory size: " << deviceProp.totalGlobalMem << std::endl;
                std::cout << " Maximum constant memory size: " << deviceProp.totalConstMem << std::endl;
                std::cout << " Maximum shared memory size per block: " << deviceProp.sharedMemPerBlock << std::endl;
                std::cout << " Maximum block dimensions: " << deviceProp.maxThreadsDim[0] << " x " <<
                                                              deviceProp.maxThreadsDim[1] << " x " <<
                                                              deviceProp.maxThreadsDim[2]) << std::endl;
                std::cout << " Maximum grid dimensions: " << deviceProp.maxGridSize[0] << " x " <<
                                                             deviceProp.maxGridSize[1] << " x " <<
                                                             deviceProp.maxGridSize[2] << std::endl;
                std::cout << " Warp size: " << deviceProp.warpSize << std::endl;
                std::cout << std::endl;
        }            
#endif  // CUDA_AVAILABLE
    } else {
        std::cout << "CUDA not available" << std::endl;
    }

    if (torch::cuda::cudnn_is_available()) {
        std::cout << "cuDNN available" << std::endl;
    } else {
        std::cout << "cuDNN not available" << std::endl;
    }

    std::cout << std::endl << "CPU" << std::endl;

    //may return 0 when not able to detect
    const auto processor_count = std::thread::hardware_concurrency();

    if (processor_count == 0) {
        std::cout << "Unable to detect number of cores";
    } else {
        std::cout << "Number of cores: " << processor_count << std::endl;
    }

    // System memory
    // This works on macOS
    int mib[2];
    int64_t physical_memory;
    size_t length;

    // Get the Physical memory size
    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    length = sizeof(int64_t);
    sysctl(mib, 2, &physical_memory, &length, NULL, 0);

    std::cout << "Physical memory: " << physical_memory / (1024*1024*1024) << "TB" << std::endl;

    return 0;
}
