#include <iostream>
#include <unistd.h>  // For pipe(), fork(), read(), write()
#include <cstring>   // For strlen()
#include <sys/wait.h> // For wait()

#define BUFFER_SIZE 1024

int main() {
    int pipefd[2];  // Array to hold file descriptors for pipe [0] for read, [1] for write
    pid_t pid;
    char writeMsg[] = "Hello from Program 1!";
    char readBuffer[BUFFER_SIZE];

    // Step 1: Create a pipe
    if (pipe(pipefd) == -1) {
        std::cerr << "Pipe creation failed!" << std::endl;
        return 1;
    }

    // Step 2: Create a child process using fork
    pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed!" << std::endl;
        return 1;
    }

    if (pid == 0) {
        // Child process (Program 2 - Receiver)
        
        // Step 3: Close the write end of the pipe, because we only read here
        close(pipefd[1]);

        // Step 4: Read the message from the pipe
        read(pipefd[0], readBuffer, BUFFER_SIZE);
        std::cout << "Program 2 received: " << readBuffer << std::endl;

        // Step 5: Close the read end of the pipe
        close(pipefd[0]);
    } else {
        // Parent process (Program 1 - Sender)
        
        // Step 3: Close the read end of the pipe, because we only write here
        close(pipefd[0]);

        // Step 4: Write a message into the pipe
        write(pipefd[1], writeMsg, strlen(writeMsg) + 1);  // +1 to include the null terminator
        std::cout << "Program 1 sent: " << writeMsg << std::endl;

        // Step 5: Close the write end of the pipe
        close(pipefd[1]);

        // Wait for the child process to finish
        wait(NULL);
    }

    return 0;
}
