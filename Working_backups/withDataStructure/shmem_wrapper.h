#pragma once
#include <string>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <unistd.h>
#include <chrono> 

namespace ipc = boost::interprocess;

class SMemory {
public:
    SMemory();  

    bool checkMemory(std::string variable);  
    void create(std::string name, unsigned int array_size);  
    void createVector(std::string name, unsigned int data_size);  

    bool update(std::string name, unsigned int arr_location, double value, unsigned int array_size);  
    double fetch(std::string name, unsigned int arr_location);  
    
    bool updateVector(std::string name, unsigned int location, double value, unsigned int array_size);  
    double fetchVector(std::string name, unsigned int location);  

private:
    struct SharedData {
        ipc::interprocess_mutex mutex;
        double data[16];  
    };
};

