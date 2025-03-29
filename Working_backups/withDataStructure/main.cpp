#include "shmem_wrapper.h"
#include <iostream>

int main() {
    srand(time(NULL));
    std::cout << "[DEBUG] Checking [Updating Values to SHM]....." << std::endl;
    SMemory sharedMemoryManager;
    std::string shm_name = "TestVector";
    std::string shm_name1 = "latency";
    while(true){
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
        double time_in_ms = duration.count();
        // double time_in_ms = static_cast<double>(duration.count());
        // std::cout << "Updating Vector data and latency..." << std::endl;
        // sharedMemoryManager.updateVector(shm_name, 1, (float)rand() / 1000, 16);
        std::cout << "Latency...: " << time_in_ms << std::endl;
        sharedMemoryManager.updateVector(shm_name1, 1, time_in_ms, 16);   // Insert value at location 0
        sharedMemoryManager.updateVector(shm_name, 1, (float)rand() / 10, 16); 
    }
    return 0;
}