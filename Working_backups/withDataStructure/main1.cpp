#include "shmem_wrapper.h"
#include <iostream>
#include <chrono>
#include <cmath>

int main() {
    std::cout << "[DEBUG] Checking [Fetching values from SHM]....." << std::endl;
    SMemory shm;
    std::cout << "Fetching shared memory latency....1...." << std::endl;
    // std::string shm_name = "latency";
    while(true){
        
        double oldTime = shm.fetchVector("latency", 1);
        double vectorData = shm.fetchVector("TestVector", 1);
         
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
        double time_in_ms = duration.count();
    
        double latency = time_in_ms - oldTime;  // Fetch value at location 0
        std::cout << "Latency: " << latency << ", Vector Data: "<< round(vectorData) << std::endl;
    } 

    return 0;
}