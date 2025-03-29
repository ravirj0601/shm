#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <iostream>

int main() 
{
    using namespace boost::interprocess;

    // Create or open a shared memory segment
    managed_shared_memory segment(open_or_create, "BoostSharedMemory", 65536);

    // Create an integer in shared memory
    int *shared_int_value = segment.find_or_construct<int>("SharedInt")(99);  // Initialize with 99
    bool *shared_bool_value = segment.find_or_construct<bool>("SharedBool")(true);  // Initialize with true
    double *shared_double_value = segment.find_or_construct<double>("SharedDouble")(100.001);  // Initialize with 100.001

    // Define custom allocator type for shared memory
    typedef managed_shared_memory::segment_manager segment_manager_t;
    typedef allocator<char, segment_manager_t> CharAllocator;
    typedef boost::interprocess::basic_string<char, std::char_traits<char>, CharAllocator> ShmemString;

    // Create or find a string in shared memory
    ShmemString *shared_string = segment.find_or_construct<ShmemString>("SharedString")(segment.get_segment_manager());
    *shared_string = "Hello from shared memory!";

    // Define an allocator type for a vector of integers in shared memory
    typedef allocator<int, segment_manager_t> IntAllocator;
    typedef boost::interprocess::vector<int, IntAllocator> ShmemIntVector;

    // Create or find a vector in shared memory
    ShmemIntVector *shared_vector = segment.find_or_construct<ShmemIntVector>("SharedVector")(segment.get_segment_manager());
    if(shared_vector->size()==0)
    {
        shared_vector->push_back(1);
        shared_vector->push_back(2);
        shared_vector->push_back(3);
    }
    
    // Print values from shared memory
    std::cout << "C++: Shared value created in shared memory." << std::endl;
    std::cout << "Shared Int Value: " << *shared_int_value << std::endl;
    std::cout << "Shared Bool Value: " << *shared_bool_value << std::endl;
    std::cout << "Shared Double Value: " << *shared_double_value << std::endl;
    std::cout << "Shared String Value: " << *shared_string << std::endl;
    
    std::cout << "Shared Vector Values: ";
    for (const int &val : *shared_vector) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
