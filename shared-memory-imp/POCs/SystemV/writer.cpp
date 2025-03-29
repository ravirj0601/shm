#include <iostream>
#include <csignal>
#include <random>
#include <chrono>
#include "shmsystemv.h" 

SharedMemoryManager manager;
bool check = true;

void handle_signal(int signal) {
    std::cout << "\n[INFO] Signal received: " << signal << ". Exiting gracefully..." << std::endl;
    manager.close_shm("latencyPOS");
    manager.close_shm("IMU");
    // shm_manager.close_fd("");
    // shm_manager.close_fd("");
    check = false;
}

int main() {
    SharedMemoryManager manager;
    
    std::string shm_name = "/test_shm";
    std::string shm_name1 = "/qtest_shm";
    std::signal(SIGINT, handle_signal);
    double IMU_roll = 0.0, IMU_pitch = 0.0;
    int IMU_heading = 0.0;
    const size_t max_size = 10;
    while(check){
        std::random_device rd;  // Random device to seed the generator
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd() 
        std::uniform_int_distribution<> distrib(100, 999);  // Define the range [1, 10]
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
        double time_in_ms = duration.count();

        std::cout << "Testing write and read operations..." << std::endl;

        double roll  = distrib(gen);
        manager.write(shm_name, 0, time_in_ms, max_size); 
        // double read_value = manager.read(shm_name, 0); 
        std::cout << "Written value: " << time_in_ms << std::endl;


        std::cout << "\nTesting enqueue and dequeue operations..." << std::endl;

        manager.enqueue(shm_name, roll, max_size);

        // double dequeued_value1 = manager.dequeue(shm_name, max_size);

        // std::cout << "Dequeued values: " << dequeued_value1 << std::endl;
        // usleep(20000);
    }

    return 0;
}
