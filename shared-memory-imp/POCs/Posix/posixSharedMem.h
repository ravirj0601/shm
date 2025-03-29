#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <string>
#include <sys/stat.h>
#include <semaphore.h>

// #define DEFAULT_SHM_NAME "/example_shm"
#define DEFAULT_MAX_SIZE 1024

struct SharedMemoryStruct {
    // pthread_mutex_t mutex;
    sem_t semaphore; 
    size_t front;
    size_t rear;
    size_t size;
    double data[];
};

class SharedMemoryManager {
public:
    SharedMemoryManager() = default;
    ~SharedMemoryManager() = default;
    void write(const std::string& shm_name, size_t index, double value, size_t size);
    double read(const std::string& shm_name, size_t index);
    void enqueue(const std::string& shm_name, double value, size_t max_size);
    double dequeue(const std::string& shm_name, size_t max_size);
    bool close_fd(const std::string &shm_name);
private:
    int open_or_create_shm(const std::string& shm_name, size_t size);
    // void initialize_mutex(SharedMemoryStruct* shm_ptr);
    void initialize_semaphore(SharedMemoryStruct* shm_ptr);
};

#endif 
