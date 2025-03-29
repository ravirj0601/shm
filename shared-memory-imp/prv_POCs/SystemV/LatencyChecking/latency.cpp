#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <iostream>
#include <chrono>
#include<unistd.h>

struct Data {
    int id;
    char name[256];
};

int main() {
    key_t key = 101;  // Key for the shared memory segment
    size_t size = sizeof(Data);  // Size of the shared memory segment

    // Create the shared memory segment
    int shmid = shmget(key, size, IPC_CREAT | 0666);
    std::cout<<shmid<<std::endl;
    if (shmid == -1) {
        std::cerr << "Error creating shared memory segment" << std::endl;
        return 1;
    }

    // Attach to the shared memory segment
    Data* data = (Data*)shmat(shmid, nullptr, 0);
    if (data == (Data*)-1) {
        std::cerr << "Error attaching to shared memory segment" << std::endl;
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10e5; i++) {
        // Populate the structure
        data->id = i;
        std::string name="Shared Memory"+std::to_string(i);
        strcpy(data->name, name.c_str());
        
        std::cout<<data->id<<std::endl;
        std::cout<<data->name<<std::endl;
        usleep(500000);
    }
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Writing From Cpp Elapsed time: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
