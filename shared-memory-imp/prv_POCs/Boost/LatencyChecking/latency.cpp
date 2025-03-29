#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <iostream>
#include<string>
#include<chrono>
#include<unistd.h>

int main()
{
    using namespace boost::interprocess;

    // Create or open a shared memory segment
    managed_shared_memory segment(open_or_create, "LatencyBoostSharedMemory", 65536);

    // Define custom allocator type for shared memory
    typedef managed_shared_memory::segment_manager segment_manager_t;
    typedef allocator<char, segment_manager_t> CharAllocator;
    typedef boost::interprocess::basic_string<char, std::char_traits<char>, CharAllocator> ShmemString;

    auto start_time=std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10e5; i++)
    {
        int *shared_int_value = segment.find_or_construct<int>("SharedInt")(0);
        *shared_int_value=i;

        ShmemString *shared_string = segment.find_or_construct<ShmemString>("SharedString")(segment.get_segment_manager());
        std::string temp_string = "Shared Memory " + std::to_string(i);
        *shared_string = temp_string.c_str();

        std::cout<<*shared_int_value<<std::endl;
        std::cout<<*shared_string<<std::endl;
        usleep(500000);
    }

    auto end_time=std::chrono::high_resolution_clock::now();

    // Calculate the duration
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Writing From Cpp Elapsed time: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
