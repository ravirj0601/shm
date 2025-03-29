#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <iostream>
#include<chrono>

using namespace boost::interprocess;

managed_shared_memory segment(open_or_create, "BoostP2", 5120);

void shared_memory(int value)
{
    int *shared_int_value = segment.find_or_construct<int>("SharedInt")(99);  
    *shared_int_value=value;
    return;
}

int main() 
{
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0;i<10e6;i++)
    {
        shared_memory(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Writing From Cpp Elapsed time: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}