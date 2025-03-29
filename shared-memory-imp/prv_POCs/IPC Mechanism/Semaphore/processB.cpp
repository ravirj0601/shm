#include <iostream>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>

const char *SEM_SIGNAL = "/sem_signal";

int main() {
    // Open semaphore
    sem_t *sem_signal = sem_open(SEM_SIGNAL, 0);

    std::cout << "Process B: Waiting for signal from Process A..." << std::endl;
    sem_wait(sem_signal); // Wait for signal

    std::cout << "Process B: Received signal. Starting work..." << std::endl;
    // Process B work

    // Cleanup
    sem_close(sem_signal);
    sem_unlink(SEM_SIGNAL);

    return 0;
}
