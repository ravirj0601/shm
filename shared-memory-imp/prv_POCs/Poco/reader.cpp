#include <Poco/SharedMemory.h>
#include <Poco/Exception.h>
#include <iostream>
#include <cstring>

const std::string SHARED_MEMORY_NAME = "PocoSharedMemory";
const size_t SIZE = 1024;  // Should match the size used in the writer program

int main() {
    try {
        // Open the shared memory segment
        Poco::SharedMemory shm(SHARED_MEMORY_NAME, SIZE, Poco::SharedMemory::AM_READ);

        // Access the shared memory
        const char* data = static_cast<const char*>(shm.begin());

        // Read from shared memory, ensuring it doesn't exceed the allocated size
        std::string readData(data, SIZE);
        
        // Find the null terminator in the string and truncate the output
        std::size_t nullPos = readData.find('\0');
        if (nullPos != std::string::npos) {
            readData = readData.substr(0, nullPos);
        }

        std::cout << "Read from shared memory: " << readData << std::endl;

    } catch (Poco::Exception& exc) {
        std::cerr << "Poco exception: " << exc.displayText() << std::endl;
        return 1;
    }
    return 0;
}
