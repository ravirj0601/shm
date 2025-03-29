#include <boost/python.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <string>
#include <vector>

using namespace boost::interprocess;

// Function to get an integer from shared memory
int get_shared_int() {
    managed_shared_memory segment(open_only, "BoostSharedMemory");
    int *shared_int_value = segment.find<int>("SharedInt").first;
    return *shared_int_value;
}

// Function to get a boolean from shared memory
bool get_shared_bool() {
    managed_shared_memory segment(open_only, "BoostSharedMemory");
    bool *shared_bool_value = segment.find<bool>("SharedBool").first;
    return *shared_bool_value;
}

// Function to get a double from shared memory
double get_shared_double() {
    managed_shared_memory segment(open_only, "BoostSharedMemory");
    double *shared_double_value = segment.find<double>("SharedDouble").first;
    return *shared_double_value;
}

// Function to get a string from shared memory
std::string get_shared_string() {
    // Open the shared memory segment
    managed_shared_memory segment(open_only, "BoostSharedMemory");

    // Define custom allocator type for shared memory
    typedef managed_shared_memory::segment_manager segment_manager_t;
    typedef allocator<char, segment_manager_t> CharAllocator;
    typedef boost::interprocess::basic_string<char, std::char_traits<char>, CharAllocator> ShmemString;

    // Find the string in shared memory
    ShmemString *shared_string = segment.find<ShmemString>("SharedString").first;

    if (shared_string) {
        // Convert shared string to std::string
        return std::string(shared_string->c_str());
    } else {
        return "";  // Return an empty string if not found
    }
}

// Function to get a vector from shared memory
std::vector<int> get_shared_vector() {
    managed_shared_memory segment(open_only, "BoostSharedMemory");

    // Define custom allocator type for shared memory
    typedef managed_shared_memory::segment_manager segment_manager_t;
    typedef allocator<int, segment_manager_t> IntAllocator;
    typedef boost::interprocess::vector<int, IntAllocator> ShmemIntVector;

    // Find the vector in shared memory
    ShmemIntVector *myVector = segment.find<ShmemIntVector>("SharedVector").first;

    if (myVector) {
        // Convert the shared memory vector to a standard std::vector
        return std::vector<int>(myVector->begin(), myVector->end());
    } else {
        return std::vector<int>();  // Return an empty vector if not found
    }
}

// Boost.Python module definition
BOOST_PYTHON_MODULE(shared_memory_module) {
    using namespace boost::python;

    // Expose functions to Python
    def("get_shared_int", get_shared_int);
    def("get_shared_bool", get_shared_bool);
    def("get_shared_double", get_shared_double);
    def("get_shared_string", get_shared_string);

    // Expose std::vector<int> to Python as a list
    class_<std::vector<int>>("IntVector")
        .def(vector_indexing_suite<std::vector<int>>());

    def("get_shared_vector", get_shared_vector);
}
