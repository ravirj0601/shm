#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>
#include<chrono>

const char *SHARED_MEMORY_NAME = "/PosixP1";
const size_t SIZE = 1024;
void shared_memory(int value)
{
    // Open shared memory object
    int shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
    }

    // Resize shared memory object
    if (ftruncate(shm_fd, SIZE) == -1) {
        perror("ftruncate");
    }

    // Memory map the shared memory object
    void *ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
    }

    // --- Write integer to shared memory ---
    int *int_ptr = (int*)ptr;
    *int_ptr = value;
    munmap(ptr, SIZE);
    close(shm_fd);
}


int main() 
{
    
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0;i<10e6;i++)
    {
        shared_memory(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    // Calculate the duration
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Writing From Cpp Elapsed time: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
