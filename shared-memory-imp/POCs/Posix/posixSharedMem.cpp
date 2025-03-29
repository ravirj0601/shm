#include "posixSharedMem.h"

void SharedMemoryManager::write(const std::string &shm_name, size_t index, double value, size_t size)
{
    if (index >= size)
    {
        std::cerr << "Error: Index out of bounds." << std::endl;
        return;
    }

    int shm_fd = open_or_create_shm(shm_name, size);
    if (shm_fd == -1)
    {
        return;
    }

    SharedMemoryStruct *shm_ptr = static_cast<SharedMemoryStruct *>(
        mmap(nullptr, sizeof(SharedMemoryStruct) + size * sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    if (shm_ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    initialize_semaphore(shm_ptr);

    if (sem_wait(&shm_ptr->semaphore) == 0)
    { // Lock semaphore
        __sync_synchronize();
        shm_ptr->data[index] = value;
        if (shm_ptr->data[index] == value)
        {
            std::cout << "[Write Success] Value " << value << " successfully written at index " << index << std::endl;
        }
        else
        {
            std::cerr << "[Write Failed] Value " << value << " not written at index " << index << ". Read value: "
                      << shm_ptr->data[index] << std::endl;
        }
        __sync_synchronize();
        sem_post(&shm_ptr->semaphore); // Unlock semaphore
    }
    else
    {
        perror("sem_wait failed");
    }

    if (munmap(shm_ptr, sizeof(SharedMemoryStruct) + size * sizeof(double)) == -1)
    {
        perror("munmap");
    }

    close(shm_fd);
}

double SharedMemoryManager::read(const std::string &shm_name, size_t index)
{
    int shm_fd = shm_open(shm_name.c_str(), O_RDWR, 0666);
    if (shm_fd == -1)
    {
        std::cerr << "[READ] Error: Shared memory does not exist." << std::endl;
        return 0;
    }

    struct stat shm_stat;
    fstat(shm_fd, &shm_stat);
    size_t shm_size = shm_stat.st_size;

    SharedMemoryStruct *shm_ptr = static_cast<SharedMemoryStruct *>(
        mmap(nullptr, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    if (shm_ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    double value = 0;
    if (index * sizeof(double) < (shm_size - sizeof(SharedMemoryStruct)))
    {
        if (sem_wait(&shm_ptr->semaphore) == 0)
        { // Lock semaphore
            __sync_synchronize();
            value = shm_ptr->data[index];
            __sync_synchronize();
            sem_post(&shm_ptr->semaphore); // Unlock semaphore
        }
        else
        {
            perror("sem_wait failed");
        }
    }
    else
    {
        std::cerr << "Error: Index out of bounds." << std::endl;
    }

    if (munmap(shm_ptr, shm_size) == -1)
    {
        perror("munmap");
    }

    close(shm_fd);
    return value;
}

void SharedMemoryManager::enqueue(const std::string &shm_name, double value, size_t max_size)
{
    int shm_fd = open_or_create_shm(shm_name, max_size);
    if (shm_fd == -1)
    {
        return;
    }

    SharedMemoryStruct *shm_ptr = static_cast<SharedMemoryStruct *>(
        mmap(nullptr, sizeof(SharedMemoryStruct) + max_size * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    if (shm_ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    initialize_semaphore(shm_ptr);

    if (sem_wait(&shm_ptr->semaphore) == 0)
    { // Lock semaphore
        __sync_synchronize();
        if ((shm_ptr->rear + 1) % max_size == shm_ptr->front)
        {
            std::cerr << "Error: Queue is full." << std::endl; // Queue is full
        }
        else
        {
            shm_ptr->data[shm_ptr->rear] = value;
            shm_ptr->rear = (shm_ptr->rear + 1) % max_size;
            std::cout << "[Enqueue] Added value " << value << std::endl;
        }
        // __sync_synchronize();
        sem_post(&shm_ptr->semaphore); // Unlock semaphore
    }
    else
    {
        perror("sem_wait failed");
    }

    if (munmap(shm_ptr, sizeof(SharedMemoryStruct) + max_size * sizeof(double)) == -1)
    {
        perror("munmap");
    }

    close(shm_fd);
}

double SharedMemoryManager::dequeue(const std::string &shm_name, size_t max_size)
{
    int shm_fd = shm_open(shm_name.c_str(), O_RDWR, 0666);
    if (shm_fd == -1)
    {
        std::cerr << "[DEQUEUE] Error: Shared memory does not exist." << std::endl;
        return 0;
    }

    struct stat shm_stat;
    fstat(shm_fd, &shm_stat);
    size_t shm_size = shm_stat.st_size;

    SharedMemoryStruct *shm_ptr = static_cast<SharedMemoryStruct *>(
        mmap(nullptr, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    if (shm_ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // sem_wait(&shm_ptr->semaphore);
    // if (shm_ptr->front == shm_ptr->rear)
    // {
    //     std::cerr << "Error: Queue is empty." << std::endl;
    //     sem_post(&shm_ptr->semaphore);
    //     return 0;
    // }
    // else
    // {
    //     double value = shm_ptr->data[shm_ptr->front];
    //     shm_ptr->front = (shm_ptr->front + 1) % max_size;
    //     std::cout << "[Dequeue] Removed value " << value << std::endl;
    //     sem_post(&shm_ptr->semaphore);
    //     return value;
    // }

    int value = 0;
    if (sem_wait(&shm_ptr->semaphore) == 0)
    { // Lock semaphore
        // __sync_synchronize();
        if (shm_ptr->front == shm_ptr->rear)
        {
            std::cerr << "Error: Queue is empty." << std::endl; // Queue is empty
        }
        else
        {
            value = shm_ptr->data[shm_ptr->front];
            shm_ptr->front = (shm_ptr->front + 1) % max_size;
            std::cout << "[Dequeue] Removed value " << value << std::endl;
        }
        // __sync_synchronize();
        sem_post(&shm_ptr->semaphore); // Unlock semaphore
        return value;
    }
    else
    {
        perror("sem_wait failed");
    }

    if (munmap(shm_ptr, shm_size) == -1)
    {
        perror("munmap");
    }

    close(shm_fd);
    return 0;
}

int SharedMemoryManager::open_or_create_shm(const std::string &shm_name, size_t size)
{
    int shm_fd = shm_open(shm_name.c_str(), O_RDWR, 0666);
    if (shm_fd == -1)
    {
        shm_fd = shm_open(shm_name.c_str(), O_CREAT | O_RDWR, 0666);
        if (shm_fd == -1)
        {
            perror("shm_open");
            return -1;
        }
        ftruncate(shm_fd, sizeof(SharedMemoryStruct) + size * sizeof(double));
    }
    return shm_fd;
}

bool SharedMemoryManager::close_fd(const std::string &shm_name)
{
    int shm_fd = shm_open(shm_name.c_str(), O_RDWR, 0666);
    if (shm_fd != -1)
    {
        SharedMemoryStruct *shm_ptr = static_cast<SharedMemoryStruct *>(
            mmap(nullptr, sizeof(SharedMemoryStruct), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
        if (shm_ptr != MAP_FAILED)
        {
            sem_destroy(&shm_ptr->semaphore); // Destroy semaphore
            munmap(shm_ptr, sizeof(SharedMemoryStruct));
        }
        close(shm_fd);
    }

    if (shm_unlink(shm_name.c_str()) == -1)
    {
        perror("Error deleting shared memory");
        return false;
    }

    std::cout << "Shared memory " << shm_name << " deleted successfully." << std::endl;
    return true;
}

void SharedMemoryManager::initialize_semaphore(SharedMemoryStruct *shm_ptr)
{
    if (sem_init(&shm_ptr->semaphore, 1, 1) == -1)
    { // 1 for inter-process, initial value 1
        perror("sem_init Failure");
        exit(EXIT_FAILURE);
    }
}
