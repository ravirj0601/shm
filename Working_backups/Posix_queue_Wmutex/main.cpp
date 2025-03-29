#include "posixSharedMem.h"
#include <chrono>
#include <math.h>
#include <iostream>

int main(int argc, char* argv[]) {
    
    double IMU_roll = 0.0, IMU_pitch = 0.0, IMU_heading = 0.0;
    
    SharedMemoryManager shm_manager;

    while(true){
        IMU_heading = round((int)rand()%360);
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
        double time_in_ms = duration.count();
        shm_manager.write("latencyPOS", 1, time_in_ms, 2);
        // shm_manager.write("IMU", 1, IMU_heading, 16);
        // shm_manager.enqueue("latencyPOS", time_in_ms,4);
        shm_manager.enqueue("IMU", IMU_heading, 50);
        std::cout << "[Updating] [IMU] Heading: " << IMU_heading << "[Latency] in microS: " << time_in_ms << std::endl;
        // usleep(200000);
        // std::cout << "[Fetched] [IMU] Heading: " << shm_manager.dequeue("IMU", 16) << std::endl << "[Latency] in microS: " << shm_manager.dequeue("latencyPOS", 4) << std::endl;
        // usleep(2000000);
    }
    shm_manager.close_fd("IMU");
    shm_manager.close_fd("latencyPOS");
    return 0;
}