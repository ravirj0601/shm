#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>

#define MAX_TEXT 100

struct Message {
    long mtype;  // Message type
    char mtext[MAX_TEXT];
};

int main() {
    key_t key = ftok("progfile", 65);  // Generate a unique key
    int msgid = msgget(key, 0666 | IPC_CREAT);  // Create a message queue

    Message msg;
    // Send messages of different types
    for (int i = 1; i <= 100; ++i) {
        msg.mtype = i % 2 + 1;  // Alternate between types 1 and 2
        snprintf(msg.mtext, MAX_TEXT, "Message number %d", i);
        msgsnd(msgid, &msg, sizeof(msg.mtext), 0);
    }

    std::cout << "Messages sent." << std::endl;
    return 0;
}
