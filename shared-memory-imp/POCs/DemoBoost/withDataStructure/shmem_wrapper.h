#pragma once
#include <string>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/deque.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <unistd.h>
#include <chrono>

namespace ipc = boost::interprocess;

class SMemory
{
public:
    SMemory();

    bool checkMemory(std::string variable);
    void create(std::string name, unsigned int array_size);
    void createVector(std::string name, unsigned int data_size);
    void createQueue(std::string name, unsigned int data_size);
    // Double type Array
    bool update(std::string name, unsigned int arr_location, double value, unsigned int array_size);
    double fetch(std::string name, unsigned int arr_location);
    // Double type Vector
    bool updateVector(std::string name, unsigned int location, double value, unsigned int array_size);
    double fetchVector(std::string name, unsigned int location);
    // Double type Queue
    bool enqueue(std::string name, double value, int array_size);
    double dequeue(std::string name);
    // For Removing sharedMemory variables
    bool removeShm(const std::string &shm_name);

private:
    struct SharedData
    {
        ipc::interprocess_mutex mutex;
        double data[16];
    };

    // struct SharedQueue
    // {
    //     ipc::interprocess_mutex mutexQue;
    //     size_t front;
    //     size_t rear;
    //     size_t size;
    //     double data[];
    // };
};
