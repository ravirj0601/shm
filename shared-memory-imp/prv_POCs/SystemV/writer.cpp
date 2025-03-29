#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <iostream>

struct Data {
    int id;
    char name[256];
};

int main() {

    // For structure data type
    #pragma region 
    key_t key1 = 65;
    int shmid1 = shmget(key1, sizeof(Data), 0666 | IPC_CREAT);
    if (shmid1 == -1) {
        std::cerr << "Error creating shared memory segment" << std::endl;
        return 1;
    }

    Data* data1 = (Data*) shmat(shmid1, nullptr, 0);
    if (data1 == (Data*) -1) {
        std::cerr << "Error attaching to shared memory segment" << std::endl;
        return 1;
    }

    // Populate the structure
    data1->id = 1;
    strcpy(data1->name, "Shared Memory");
    std::cout<<"String Data is written\n";
    shmdt(data1);
    #pragma endregion


    // For int data type
    #pragma region 
    // For structure data type
    key_t key2 = 66;
    int shmid2 = shmget(key2, sizeof(int), 0666 | IPC_CREAT);
    if (shmid2 == -1) {
        std::cerr << "Error creating shared memory segment" << std::endl;
        return 1;
    }

    int* data2 = (int*) shmat(shmid2, nullptr, 0);
    if (data2 == (int*) -1) {
        std::cerr << "Error attaching to shared memory segment" << std::endl;
        return 1;
    }

    // Populate the structure
    *data2=1001;
    std::cout<<"Ingteger Data is written\n";
    shmdt(data2);
    #pragma endregion


    // For integer array data type
    #pragma region 

    key_t key3 = 67;
    int shmid3 = shmget(key3, 5*sizeof(int), 0666 | IPC_CREAT);
    if (shmid3 == -1) {
        std::cerr << "Error creating shared memory segment" << std::endl;
        return 1;
    }

    int* data3 = (int*) shmat(shmid3, nullptr, 0);
    if (data3 == (int*) -1) {
        std::cerr << "Error attaching to shared memory segment" << std::endl;
        return 1;
    }

    // Populate the structure
    for(int i=0;i<5;i++)
    {
        data3[i]=i+100;
        
    }
    std::cout<<"Integer Array Data is written\n";
    shmdt(data3);
    #pragma endregion


    // For character array data type
    #pragma region 
    // For structure data type
    key_t key4 = 68;
    int shmid4 = shmget(key4, 5*sizeof(char), 0666 | IPC_CREAT);
    if (shmid4 == -1) {
        std::cerr << "Error creating shared memory segment" << std::endl;
        return 1;
    }

    char* data4 = (char*) shmat(shmid4, nullptr, 0);
    if (data4 == (char*) -1) {
        std::cerr << "Error attaching to shared memory segment" << std::endl;
        return 1;
    }

    // Populate the structure
    for(int i=0;i<5;i++)
    {
        data4[i]='a'+i;
    }
    std::cout<<"Character Array Data is written\n";
    shmdt(data4);
    #pragma endregion



    return 0;
}
