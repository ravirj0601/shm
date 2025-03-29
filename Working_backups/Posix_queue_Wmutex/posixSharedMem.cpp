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

    SharedMemoryStruct *shm_ptr = static_cast<SharedMemoryStruct *>(mmap(nullptr, sizeof(SharedMemoryStruct) + size * sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    if (shm_ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    initialize_mutex(shm_ptr);

    pthread_mutex_lock(&shm_ptr->mutex);
    shm_ptr->data[index] = value;
    // std::cout << "[Update] value " << value << " Index " << index << std::endl;
    pthread_mutex_unlock(&shm_ptr->mutex);

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
        std::cerr << "Error: Shared memory does not exist." << std::endl;
        return 0;
    }

    struct stat shm_stat;
    fstat(shm_fd, &shm_stat);
    size_t shm_size = shm_stat.st_size;

    SharedMemoryStruct *shm_ptr = static_cast<SharedMemoryStruct *>(mmap(nullptr, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    if (shm_ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    if (index * sizeof(double) >= (shm_size - sizeof(SharedMemoryStruct)))
    {
        std::cerr << "Error: Index out of bounds." << std::endl;
        return 0;
    }

    pthread_mutex_lock(&shm_ptr->mutex);
    double value = shm_ptr->data[index];
    // std::cout << "[Read] value " << value << " Index " << index << std::endl;
    pthread_mutex_unlock(&shm_ptr->mutex);

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
        mmap(nullptr, sizeof(SharedMemoryStruct) + max_size * sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));

    if (shm_ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    pthread_mutex_lock(&shm_ptr->mutex);
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
    pthread_mutex_unlock(&shm_ptr->mutex);

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
        std::cerr << "Error: Shared memory does not exist." << std::endl;
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

    pthread_mutex_lock(&shm_ptr->mutex);
    if (shm_ptr->front == shm_ptr->rear)
    {
        std::cerr << "Error: Queue is empty." << std::endl; // Queue is empty
        pthread_mutex_unlock(&shm_ptr->mutex);
        return 0;
    }
    else
    {
        double value = shm_ptr->data[shm_ptr->front];
        shm_ptr->front = (shm_ptr->front + 1) % max_size;
        pthread_mutex_unlock(&shm_ptr->mutex);
        std::cout << "[Dequeue] Removed value " << value << std::endl;
        return value;
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
        std::cout << "SHM Created: " << shm_fd << std::endl;
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
    if (shm_unlink(shm_name.c_str()) == -1)
    {
        perror("Error deleting shared memory");
        return false;
    }

    std::cout << "Shared memory " << shm_name << " deleted successfully." << std::endl;
    return true;
}

void SharedMemoryManager::initialize_mutex(SharedMemoryStruct *shm_ptr)
{
    static pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shm_ptr->mutex, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);
}
