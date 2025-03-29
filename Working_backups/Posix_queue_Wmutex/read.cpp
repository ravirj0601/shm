#include "posixSharedMem.h"
#include <chrono>

#include <iostream>

int main(int argc, char* argv[]) {
    
    double IMU_roll = 0.0, IMU_pitch = 0.0, IMU_heading = 0.0;
    
    SharedMemoryManager shm_manager;

    while(true){
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
        double time_in_ms = duration.count();
        double old_time = shm_manager.read("latencyPOS", 1);
        // int heading = shm_manager.read("IMU", 1);
        // double old_time = shm_manager.dequeue("latencyPOS",2);
        int heading = shm_manager.dequeue("IMU", 50);
        std::cout << " [IMU] Heading: " << heading << "[Latency] in microS: " << old_time << std::endl;
        int latency = time_in_ms - old_time;
        std::cout << "Latency:- " << latency << std::endl; 
        // usleep(20000);
    } 
    return 0;
}