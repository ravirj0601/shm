#include<sys/shm.h>
#include<iostream>
#include<chrono>

int shared_memory(int key,int val)
{
    int shmid=shmget(key,1024,0666|IPC_CREAT);
    //std::cout<<shmid<<std::endl;
    if (shmid == -1) {
        std::cerr << "Error creating shared memory segment" << std::endl;
        return 1;
    }

    int* data = (int*)shmat(shmid, nullptr, 0);
    if (data== (int*)-1) {
        std::cerr << "Error attaching to shared memory segment" << std::endl;
        return 1;
    }
    //std::cout<<"Writing the data\n";
    *data=val;
    int shmdtId=shmdt(data);
    if (shmdtId== -1) {
        std::cerr << "Error at de-attaching to shared memory segment" << std::endl;
        return 1;
    }
    return 0;

}
int main()
{
    int key=2011;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0;i<10e6;i++)
    {
        shared_memory(key,i);
    }
    auto end = std::chrono::high_resolution_clock::now();
        // Calculate the duration
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Writing From Cpp Elapsed time: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}