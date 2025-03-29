#include <boost/python.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <string>
#include <vector>

using namespace boost::interprocess;

// Open the shared memory segment
managed_shared_memory segment(open_only, "LatencyBoostSharedMemory");

// Define custom allocator type for shared memory
typedef managed_shared_memory::segment_manager segment_manager_t;
typedef allocator<char, segment_manager_t> CharAllocator;
typedef boost::interprocess::basic_string<char, std::char_traits<char>, CharAllocator> ShmemString;

// Function to get an integer from shared memory
int get_shared_int()
{
    int *shared_int_value = segment.find<int>("SharedInt").first;
    return *shared_int_value;
}

// Function to get a string from shared memory
std::string get_shared_string()
{

    // Find the string in shared memory
    ShmemString *shared_string = segment.find<ShmemString>("SharedString").first;
    if (shared_string)
    {
        // Convert shared string to std::string
        return std::string(shared_string->c_str());
    }
    else
    {
        return ""; // Return an empty string, if not found
    }
}

BOOST_PYTHON_MODULE(latency_shared_memory_module)
{
    using namespace boost::python;
    // Expose functions to Python
    def("get_shared_int", get_shared_int);
    def("get_shared_string", get_shared_string);
}