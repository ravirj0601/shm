#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>

const char *SHARED_MEMORY_NAME = "/PosixSharedMemory";
const size_t SIZE = 4096;

int main() 
{
    // Open shared memory object
    int shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    // Resize shared memory object
    if (ftruncate(shm_fd, SIZE) == -1) {
        perror("ftruncate");
        return 1;
    }

    // Memory map the shared memory object
    void *ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // --- Write integer to shared memory ---
    int *int_ptr = (int*)ptr;
    *int_ptr = 10;
    std::cout << "Wrote integer: " << *int_ptr << std::endl;

    // Move the pointer forward by the size of an int
    int_ptr++;

    // --- Write float to shared memory ---
    float *float_ptr = (float*)int_ptr;
    *float_ptr = 10.6f;
    std::cout << "Wrote float: " << *float_ptr << std::endl;

    // Move the pointer forward by the size of a float
    float_ptr++;

    // --- Write bool to shared memory ---
    bool *bool_ptr = (bool*)float_ptr;
    *bool_ptr = true;
    std::cout << "Wrote bool: " << (*bool_ptr ? "true" : "false") << std::endl;

    // Move the pointer forward by the size of a bool
    bool_ptr++;

    // --- Write char to shared memory ---
    char *char_ptr = (char*)bool_ptr;
    *char_ptr = 'A';
    std::cout << "Wrote char: " << *char_ptr << std::endl;

    // Move the pointer forward by the size of a char
    char_ptr++;

    // --- Write char array to shared memory ---
    const char *char_array = "Hello";
    strcpy(char_ptr, char_array);  // Copy a string into the memory
    std::cout << "Wrote char array: " << char_ptr << std::endl;

    // Move the pointer forward by the size of the char array
    char_ptr += strlen(char_array) + 1;  // +1 for the null terminator

    // Unmap the memory and close the shared memory file descriptor
    munmap(ptr, SIZE);
    close(shm_fd);

    return 0;
}
