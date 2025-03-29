#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>
#include<chrono>
#include<unistd.h>

const char *SHARED_MEMORY_NAME = "/LatencyPosixSharedMemory";
struct Data
{
    int id;
    char name[256];
};

int main() 
{
    const size_t SIZE = sizeof(Data);
    // Open shared memory object
    int shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
    std::cout<<shm_fd<<std::endl;
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    // Resize shared memory object
    if (ftruncate(shm_fd, SIZE) == -1) {
        perror("ftruncate");
        return 1;
    }

    // Memory map the shared memory object
    void *ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    Data *data = (Data*)ptr;
    auto start_time=std::chrono::high_resolution_clock::now();
    for(int i=0;i<100000;i++)
    {
        data->id = i;
        std::string name="Shared Memory"+std::to_string(i);
        strcpy(data->name,name.c_str());

        std::cout<<data->id<<std::endl;
        std::cout<<data->name<<std::endl;
        usleep(500000);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
     // Calculate the duration
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Writing From Cpp Elapsed time: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
