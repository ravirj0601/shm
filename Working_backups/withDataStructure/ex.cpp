#include <iostream>
#include <unistd.h>
#include <chrono>

int main() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    std::cout << "Current time in milliseconds since epoch: " << duration.count() << " ms" << std::endl;
    usleep(1000);
    auto now1 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(now1.time_since_epoch());
    std::cout << "Current time in milliseconds since epoch: " << duration2.count() << " ms" << std::endl;
    int diff = duration2.count() - duration.count();
    std::cout << "Time-Difference in milliseconds: " << diff << " ms" << std::endl;
    return 0;
}