#include "posixSharedMem.h"
#include <chrono>
#include <math.h>
#include <iostream>
#include <csignal>
#include <random>
SharedMemoryManager shm_manager;
bool check = true;

void handle_signal(int signal) {
    std::cout << "\n[INFO] Signal received: " << signal << ". Exiting gracefully..." << std::endl;
    shm_manager.close_fd("latencyPOS");
    shm_manager.close_fd("IMU");
    // shm_manager.close_fd("");
    // shm_manager.close_fd("");
    check = false;
}

int main(int argc, char* argv[]) {
    std::cout << "Testing ...-->" << std::endl;
    std::signal(SIGINT, handle_signal);
    double IMU_roll = 0.0, IMU_pitch = 0.0;
    int IMU_heading = 0.0;
    // SharedMemoryManager shm_manager;
    std::random_device rd;  // Random device to seed the generator
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd() 
    std::uniform_int_distribution<> distrib(100, 999);  // Define the range [1, 10]

    while(check){
        IMU_heading = distrib(gen);
        // std::cout << "Testing ...-->" << std::endl;
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
        double time_in_ms = duration.count();
        shm_manager.write("latencyPOS", 1, time_in_ms, 2);
        // std::cout << "Testing ...-->" << std::endl;
        // shm_manager.write("IMU", 1, IMU_heading, 16);
        // shm_manager.enqueue("latencyPOS", time_in_ms,4);
        shm_manager.enqueue("IMU", IMU_heading, 50);
        // std::cout << "Testing ...-->" << std::endl;
        std::cout << "[Updating] [IMU] Heading: " << IMU_heading << "[Latency] in microS: " << time_in_ms << std::endl;
        // std::cout << "[Fetched] [IMU] Heading: " << shm_manager.dequeue("IMU", 16) << std::endl ; //<< "[Latency] in microS: " << shm_manager.read("latencyPOS", 1) << std::endl;
        // usleep(20000);
        // usleep(2000000);
    }
    // shm_manager.close_fd("IMU");
    // shm_manager.close_fd("latencyPOS");
    return 0;
}
