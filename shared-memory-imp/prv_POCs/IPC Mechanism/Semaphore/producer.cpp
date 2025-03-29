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
    int shm_fd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(SharedBuffer));
    SharedBuffer *shared_buffer = (SharedBuffer *)mmap(0, sizeof(SharedBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Create semaphores
    sem_t *sem_empty = sem_open(SEM_EMPTY, O_CREAT, 0666, BUFFER_SIZE);
    sem_t *sem_full = sem_open(SEM_FULL, O_CREAT, 0666, 0);

    for (int i = 0; i < 1000; ++i) {
        sem_wait(sem_empty); // Wait for an empty slot

        shared_buffer->buffer[shared_buffer->count++] = i;
        std::cout << "Produced: " << i << std::endl;

        sem_post(sem_full); // Signal that there is a new item
        sleep(1); // Simulate production time
    }

    // Cleanup
    sem_close(sem_empty);
    sem_close(sem_full);
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);
    munmap(shared_buffer, sizeof(SharedBuffer));
    shm_unlink(SHARED_MEM_NAME);

    return 0;
}
