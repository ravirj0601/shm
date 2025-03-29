#include <Poco/SharedMemory.h>
#include <Poco/Exception.h>
#include <iostream>
#include <cstring>

const std::string SHARED_MEMORY_NAME = "PocoSharedMemory";
const size_t SIZE = 1024; // 1 KB

int main() {
    try {
        // Create or open the shared memory segment
        Poco::SharedMemory shm(SHARED_MEMORY_NAME, SIZE, Poco::SharedMemory::AM_WRITE);

        // Write data to shared memory
        const char* message = "Hello from C++!";
        std::memcpy(shm.begin(), message, std::strlen(message) + 1); // +1 for null-terminator

        std::cout << "Data written to shared memory: " << message << std::endl;
    } catch (Poco::Exception& e) {
        std::cerr << "Poco exception: " << e.displayText() << std::endl;
        return 1;
    } catch (std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
        return 1;
    }

    return 0;
}
