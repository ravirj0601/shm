#include "shmsystemv.h"

key_t SharedMemoryManager::generate_key(const std::string &shm_name) {
    return ftok(shm_name.c_str(), 65); 
}

int SharedMemoryManager::open_or_create_shm(const std::string &shm_name, size_t size) {
    key_t key = generate_key(shm_name);
    int shm_id = shmget(key, sizeof(SharedMemoryStruct) + size * sizeof(double), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget");
        return -1;
    }
    
    SharedMemoryStruct *shm_ptr = (SharedMemoryStruct*)shmat(shm_id, nullptr, 0);
    if (shm_ptr == (void*)-1) {
        perror("shmat");
        return -1;
    }

    shm_ptr->sem_id = semget(key, 1, IPC_CREAT | 0666);
    if (shm_ptr->sem_id == -1) {
        perror("semget");
        return -1;
    }

    initialize_semaphore(shm_ptr->sem_id);
    shmdt(shm_ptr); 
    return shm_id;
}

void SharedMemoryManager::initialize_semaphore(int sem_id) {
    semctl(sem_id, 0, SETVAL, 1); 
}

void SharedMemoryManager::lock_semaphore(int sem_id) {
    struct sembuf sb = {0, -1, 0}; 
    semop(sem_id, &sb, 1);
}

void SharedMemoryManager::unlock_semaphore(int sem_id) {
    struct sembuf sb = {0, 1, 0}; 
    semop(sem_id, &sb, 1);
}

void SharedMemoryManager::write(const std::string &shm_name, size_t index, double value, size_t size) {
    int shm_id = open_or_create_shm(shm_name, size);
    if (shm_id == -1) {
        return;
    }
    
    SharedMemoryStruct *shm_ptr = (SharedMemoryStruct*)shmat(shm_id, nullptr, 0);
    if (shm_ptr == (void*)-1) {
        perror("shmat");
        return;
    }

    if (index >= size) {
        std::cerr << "Error: Index out of bounds." << std::endl;
        shmdt(shm_ptr);
        return;
    }

    lock_semaphore(shm_ptr->sem_id);
    shm_ptr->data[index] = value;
    if (shm_ptr->data[index] == value) {
        std::cout << "Value " << value << " successfully written at index " << index << "." << std::endl;
    } else {
        std::cerr << "Error: Failed to write value to shared memory." << std::endl;
    }
    
    unlock_semaphore(shm_ptr->sem_id);

    shmdt(shm_ptr);
}

double SharedMemoryManager::read(const std::string &shm_name, size_t index) {
    key_t key = generate_key(shm_name);
    int shm_id = shmget(key, 0, 0666);
    if (shm_id == -1) {
        std::cerr << "[READ] Error: Shared memory does not exist." << std::endl;
        return 0;
    }

    SharedMemoryStruct *shm_ptr = (SharedMemoryStruct*)shmat(shm_id, nullptr, 0);
    if (shm_ptr == (void*)-1) {
        perror("shmat");
        return 0;
    }

    double value = 0;
    if (index < 1024) {
        lock_semaphore(shm_ptr->sem_id);
        value = shm_ptr->data[index];
        unlock_semaphore(shm_ptr->sem_id);
    } else {
        std::cerr << "Error: Index out of bounds." << std::endl;
    }

    shmdt(shm_ptr);
    return value;
}

void SharedMemoryManager::enqueue(const std::string &shm_name, double value, size_t max_size) {
    int shm_id = open_or_create_shm(shm_name, max_size);
    if (shm_id == -1) {
        return;
    }

    SharedMemoryStruct *shm_ptr = (SharedMemoryStruct*)shmat(shm_id, nullptr, 0);
    if (shm_ptr == (void*)-1) {
        perror("shmat");
        return;
    }

    lock_semaphore(shm_ptr->sem_id);
    if ((shm_ptr->rear + 1) % max_size == shm_ptr->front) {
        std::cerr << "Error: Queue is full." << std::endl;
    } else {
        shm_ptr->data[shm_ptr->rear] = value;
        shm_ptr->rear = (shm_ptr->rear + 1) % max_size;
    }
    unlock_semaphore(shm_ptr->sem_id);

    shmdt(shm_ptr);
}

double SharedMemoryManager::dequeue(const std::string &shm_name, size_t max_size) {
    key_t key = generate_key(shm_name);
    int shm_id = shmget(key, 0, 0666);
    if (shm_id == -1) {
        std::cerr << "[DEQUEUE] Error: Shared memory does not exist." << std::endl;
        return 0;
    }

    SharedMemoryStruct *shm_ptr = (SharedMemoryStruct*)shmat(shm_id, nullptr, 0);
    if (shm_ptr == (void*)-1) {
        perror("shmat");
        return 0;
    }

    double value = 0;
    lock_semaphore(shm_ptr->sem_id);
    if (shm_ptr->front == shm_ptr->rear) {
        std::cerr << "Error: Queue is empty." << std::endl;
    } else {
        value = shm_ptr->data[shm_ptr->front];
        shm_ptr->front = (shm_ptr->front + 1) % max_size;
    }
    unlock_semaphore(shm_ptr->sem_id);

    shmdt(shm_ptr);
    return value;
}

bool SharedMemoryManager::close_shm(const std::string &shm_name) {
    key_t key = generate_key(shm_name);
    int shm_id = shmget(key, 0, 0666);
    if (shm_id != -1) {
        SharedMemoryStruct *shm_ptr = (SharedMemoryStruct*)shmat(shm_id, nullptr, 0);
        if (shm_ptr != (void*)-1) {
            semctl(shm_ptr->sem_id, 0, IPC_RMID); // Remove semaphore
            shmdt(shm_ptr);
        }
        shmctl(shm_id, IPC_RMID, nullptr); // Remove shared memory
        std::cout << "Shared memory " << shm_name << " deleted successfully." << std::endl;
        return true;
    }
    return false;
}
