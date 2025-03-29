#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>
#include<chrono>

const char *SHARED_MEMORY_NAME = "/PosixP2";
const size_t SIZE = 1024;

void shared_memory(int shm_fd,int value)
{
    // Memory map the shared memory object
    void *ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
    }

    // --- Write integer to shared memory ---
    int *int_ptr = (int*)ptr;
    *int_ptr = value;
    munmap(ptr, SIZE);
    
}


int main() 
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

    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0;i<10e5;i++)
    {
        shared_memory(shm_fd,i);
    }
    close(shm_fd);
    auto end = std::chrono::high_resolution_clock::now();
    // Calculate the duration
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Writing From Cpp Elapsed time: " << elapsed.count() << " seconds" << std::endl;
    
    return 0;
}
