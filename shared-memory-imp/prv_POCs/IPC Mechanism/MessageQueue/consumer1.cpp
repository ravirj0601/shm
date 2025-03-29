#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
#include <cstring>

#define MAX_TEXT 100

struct Message {
    long mtype;  // Message type
    char mtext[MAX_TEXT];
};

int main() {
    key_t key = ftok("progfile", 65);  // Generate a unique key
    int msgid = msgget(key, 0666 | IPC_CREAT);  // Create a message queue

    Message msg;
    while (true) {
        ssize_t result = msgrcv(msgid, &msg, sizeof(msg.mtext), 1, IPC_NOWAIT);
        if (result == -1) {
            std::cerr << "No more messages of type 1." << std::endl;
            break;
        }

        std::cout << "Process A received: " << msg.mtext << std::endl;
    }

    // Cleanup
    msgctl(msgid, IPC_RMID, NULL);  // Remove the message queue
    return 0;
}
