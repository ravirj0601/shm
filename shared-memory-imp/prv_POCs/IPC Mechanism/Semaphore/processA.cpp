#include <iostream>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>

const char *SEM_SIGNAL = "/sem_signal";

int main() {
    // Create semaphore
    sem_t *sem_signal = sem_open(SEM_SIGNAL, O_CREAT, 0666, 0);

    std::cout << "Process A: Doing some work..." << std::endl;
    sleep(2); // Simulate work

    std::cout << "Process A: Signaling Process B to start." << std::endl;
    sem_post(sem_signal); // Signal Process B

    // Cleanup
    sem_close(sem_signal);

    return 0;
}
