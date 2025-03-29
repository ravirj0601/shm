#include "shmem_wrapper.h"
#include <iostream>
#include <csignal>
#include <random>

SMemory sharedMemoryManager;
bool check = true;

void handle_signal(int signal) {
    std::cout << "\n[INFO] Signal received: " << signal << ". Exiting gracefully..." << std::endl;
    sharedMemoryManager.removeShm("TestVector");
    sharedMemoryManager.removeShm("latencyVec");
    sharedMemoryManager.removeShm("latency");
    sharedMemoryManager.removeShm("TestQueue");
    check = false;
}

int main() {
    srand(time(NULL));
    // std::cout << "[DEBUG] Checking [Updating Values to SHM]....." << std::endl;
    
    std::signal(SIGINT, handle_signal);
    std::random_device rd;  // Random device to seed the generator
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
   
    std::uniform_int_distribution<> distrib(1, 10);  // Define the range [1, 10]
    std::uniform_int_distribution<> distrib1(100, 1000);

    std::string shm_name = "TestVector"; //Vector SHM name
    std::string shm_name2 = "latencyVec";   //Vector Latency SHM name
    std::string shm_name1 = "latency";  //Array double SHM name
    std::string shm_name3 = "TestQueue"; //Queue
    while(check){
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
        double time_in_ms = duration.count();
        int IMU_Heading = distrib(gen);
        double IMU_Pitch = distrib1(gen);

        std::cout << "Latency: " << time_in_ms << ", Heading: " << IMU_Heading << ", Pitch: "<< IMU_Pitch << std::endl;
        sharedMemoryManager.update(shm_name1, 1, time_in_ms, 16);
        sharedMemoryManager.updateVector(shm_name, 1, IMU_Heading, 16);
        sharedMemoryManager.enqueue(shm_name3, IMU_Pitch, 50);
        // std::cout << "deque: " << sharedMemoryManager.dequeue(shm_name3) << std::endl;
        usleep(100000);
    }
    return 0;
}