#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>

const char *SHARED_MEM_NAME = "/my_shared_memory";
const char *SEM_EMPTY = "/sem_empty";
const char *SEM_FULL = "/sem_full";
const int BUFFER_SIZE = 10;

struct SharedBuffer {
    int buffer[BUFFER_SIZE];
    int count;
};

int main() {
    // Open shared memory
    int shm_fd = shm_open(SHARED_MEM_NAME, O_RDWR, 0666);
    SharedBuffer *shared_buffer = (SharedBuffer *)mmap(0, sizeof(SharedBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Open semaphores
    sem_t *sem_empty = sem_open(SEM_EMPTY, 0);
    sem_t *sem_full = sem_open(SEM_FULL, 0);

    for (int i = 0; i < 10; ++i) {
        sem_wait(sem_full); // Wait for a filled slot

        int item = shared_buffer->buffer[--shared_buffer->count];
        std::cout << "Consumed: " << item << std::endl;

        sem_post(sem_empty); // Signal that there is an empty slot
        sleep(2); // Simulate consumption time
    }

    // Cleanup
    sem_close(sem_empty);
    sem_close(sem_full);
    munmap(shared_buffer, sizeof(SharedBuffer));

    return 0;
}
