#include<sys/shm.h>
#include<iostream>
#include<chrono>

void shared_memory(int shmid,int val)
{

    int* data = (int*)shmat(shmid, nullptr, 0);
    if (data== (int*)-1) {
        std::cerr << "Error attaching to shared memory segment" << std::endl;
        return;
    }
    //std::cout<<"Writing the data\n";
    *data=val;
    int shmdtId=shmdt(data);
    if (shmdtId== -1) {
        std::cerr << "Error at de-attaching to shared memory segment" << std::endl;
        return;
    }
    return;

}
int main()
{
    int key=3011;
    int shmid=shmget(key,4,0666|IPC_CREAT);
    //std::cout<<shmid<<std::endl;
    if (shmid == -1) {
        std::cerr << "Error creating shared memory segment" << std::endl;
        return 1;
    }
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0;i<10e4;i++)
    {
        shared_memory(shmid,i);
    }
    auto end = std::chrono::high_resolution_clock::now();
        // Calculate the duration
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Writing From Cpp Elapsed time: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}