#include "shmem_wrapper.h"
#include <iostream>
#include <chrono>
#include <cmath>

int main() {
    std::cout << "[DEBUG] Checking [Fetching values from SHM]....." << std::endl;
    SMemory shm;
    std::cout << "Fetching shared memory latency....1...." << std::endl;
    while(true){
        
        double oldTime = shm.fetch("latency", 1);
        int vectorData = shm.fetchVector("TestVector", 1);
        double queueData = shm.dequeue("TestQueue");
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
        double time_in_ms = duration.count();
        std::cout << "Fetched Time: " << oldTime << ", Local Time: "<< time_in_ms << std::endl;
        double latency = time_in_ms - oldTime;
        std::cout << "Latency: " << latency << ", Vector Data: "<< round(vectorData) << ", Queue Data: " << queueData << std::endl;
        usleep(100000);
    } 

    return 0;
}