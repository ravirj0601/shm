#ifndef SHMSYSTEMV_H
#define SHMSYSTEMV_H

#include <string>
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <cerrno>

struct SharedMemoryStruct {
    double data[1024];
    int front = 0;
    int rear = 0;
    int sem_id; 
};

class SharedMemoryManager {
public:
    void write(const std::string &shm_name, size_t index, double value, size_t size);
    double read(const std::string &shm_name, size_t index);
    void enqueue(const std::string &shm_name, double value, size_t max_size);
    double dequeue(const std::string &shm_name, size_t max_size);
    int open_or_create_shm(const std::string &shm_name, size_t size);
    bool close_shm(const std::string &shm_name);
    void initialize_semaphore(int sem_id);
    void lock_semaphore(int sem_id);
    void unlock_semaphore(int sem_id);
    
private:
    key_t generate_key(const std::string &shm_name);
};

#endif 