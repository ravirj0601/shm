#include "shmem_wrapper.h"
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <iostream>
#include <cmath>

#define DEFAULT_ARRAY_SIZE pow(2, 4)
#define IMU_ARRAY_SIZE DEFAULT_ARRAY_SIZE
#define PRESSURESENSOR_ARRAY_SIZE DEFAULT_ARRAY_SIZE
#define GPS_ARRAY_SIZE DEFAULT_ARRAY_SIZE
#define DVL_ARRAY_SIZE DEFAULT_ARRAY_SIZE
#define SETPOINTS_ARRAY_SIZE DEFAULT_ARRAY_SIZE
#define SMCU_ARRAY_SIZE DEFAULT_ARRAY_SIZE
#define HMU_ARRAY_SIZE DEFAULT_ARRAY_SIZE

bool SMemory::checkMemory(std::string variable)
{
    std::string path = "/dev/shm/" + variable;
    return access(path.c_str(), F_OK) != -1;
}

bool SMemory::removeShm(const std::string &shm_name)
{
    if (ipc::shared_memory_object::remove(shm_name.c_str()))
    {
        std::cout << "Successfully removed shared memory: " << shm_name << std::endl;
        return true;
    }
    else
    {
        std::cout << "Failed to remove shared memory or it doesn't exist: " << shm_name << std::endl;
        return false;
    }
}

/**
 * @brief Utility function to create/allocate shared memory with a mutex for synchronization
 */
void SMemory::create(std::string name, unsigned int data_size)
{
    if (checkMemory(name))
        return;

    ipc::managed_shared_memory managed_shm{ipc::open_or_create, name.c_str(), 1024};
    if (!managed_shm.find<SharedData>("SharedData").first)
        managed_shm.construct<SharedData>("SharedData")();
}

void SMemory::createVector(std::string name, unsigned int data_size)
{
    try
    {
        if (checkMemory(name))
        {
            std::cout << "Shared memory already exists: " << name << std::endl;
            return;
        }
        std::cout << "[DEBUG] Trying to create VectorSHM: " << std::endl;

        ipc::managed_shared_memory managed_shm{ipc::create_only, name.c_str(), 1024 * 1024}; // data_size * 1024};

        std::cout << "[DEBUG] Created shared memory: " << name << " with size: " << data_size << std::endl;

        using ShmemAllocator = ipc::allocator<double, ipc::managed_shared_memory::segment_manager>;
        using vec_t = ipc::vector<double, ShmemAllocator>;

        const ShmemAllocator alloc_inst(managed_shm.get_segment_manager());

        managed_shm.construct<vec_t>("SharedVector")(alloc_inst);

        managed_shm.construct<ipc::interprocess_mutex>("SharedMutex")();
    }
    catch (const boost::interprocess::interprocess_exception &e)
    {
        std::cerr << "Error creating shared memory: " << e.what() << std::endl;
        throw;
    }
}

/**
 * @brief Utility function to fetch value of some shared memory variable
 */
double SMemory::fetch(std::string name, unsigned int arr_location)
{
    if (!checkMemory(name))
        return 0;
    try
    {
        ipc::managed_shared_memory managed_shm{ipc::open_only, name.c_str()};

        // If no exception, the mutex will locked successfully
        // std::cout << "Mutex locked successfully." << std::endl;
        std::pair<SharedData *, std::size_t> p = managed_shm.find<SharedData>("SharedData");

        if (p.first)
        {
            
             __sync_synchronize();

            ipc::scoped_lock<ipc::interprocess_mutex> lock(p.first->mutex);

             __sync_synchronize();

            return p.first->data[arr_location - 1];
        }
        else
        {
            std::cerr << "SharedMemory not found..." << std::endl;
        }
    }
    catch (const boost::interprocess::lock_exception &e)
    {
        std::cerr << "Failed to lock mutex: " << e.what() << std::endl;
        // Handle the error, possibly return a default value or rethrow the exception
        return 0; // Default return value on failure
    }
    catch (const std::exception &e)
    {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 0;
    }
    return 0;
}

/**
 * @brief Utility function to update shared memory variable value
 */
bool SMemory::update(std::string name, unsigned int arr_location,
                     double value, unsigned int array_size)
{
    /* Sanity check */
    if (arr_location > array_size)
        return false;

    /* Create the shared memory if it doesn't exist */
    if (!checkMemory(name))
    {
        std::cout << "Memory doesn't exist, Creating....." << std::endl;
        this->create(name, array_size);
    }
    ipc::managed_shared_memory managed_shm{ipc::open_only, name.c_str()};
    std::pair<SharedData *, std::size_t> p = managed_shm.find<SharedData>("SharedData");
    // std::cout << "Testing double array: " << p.first << std::endl;
    if (p.first)
    {
        ipc::scoped_lock<ipc::interprocess_mutex> lock(p.first->mutex);
        p.first->data[arr_location - 1] = value;
        // std::cout << "[Test] Data Being Stored in Double array..." << std::endl;
        return true;
    }
    else
    {
        std::cout << "Returning False not able to find Shared Data Unable to update....." << std::endl;
    }
    return false;
}

bool SMemory::updateVector(std::string name, unsigned int location, double value, unsigned int array_size)
{
    try
    {
        // std::cout << "Checking if shared memory exists: " << name << std::endl;

        if (!checkMemory(name))
        {
            std::cout << "Shared memory doesn't exist. Creating new shared memory: " << name << std::endl;
            this->createVector(name, array_size * sizeof(double) + sizeof(ipc::interprocess_mutex));
        }

        // std::cout << "Opening existing shared memory: " << name << std::endl;
        ipc::managed_shared_memory managed_shm{ipc::open_only, name.c_str()};

        using vec_t = ipc::vector<double, ipc::allocator<double, ipc::managed_shared_memory::segment_manager>>;

        std::pair<vec_t *, std::size_t> p = managed_shm.find<vec_t>("SharedVector");
        // std::cout << "Test." << p.first << std::endl;
        if (p.first)
        {
            // std::cout << "Found vector in shared memory." << std::endl;
            std::pair<ipc::interprocess_mutex *, std::size_t> mutex_pair = managed_shm.find<ipc::interprocess_mutex>("SharedMutex");
            if (mutex_pair.first)
            {
                // std::cout << "Found mutex in shared memory. Locking mutex..." << std::endl;
                ipc::scoped_lock<ipc::interprocess_mutex> lock(*mutex_pair.first);

                if (location >= p.first->size())
                {
                    // std::cout << "Resizing vector to fit location: " << location << std::endl;
                    p.first->resize(location + 1);
                }
                (*p.first)[location] = value;

                // std::cout << "Successfully updated shared memory at location: " << location << std::endl;
                return true;
            }
            else
            {
                std::cerr << "Error: Mutex not found in shared memory!" << std::endl;
            }
        }
        else
        {
            std::cerr << "Error: Vector not found in shared memory!" << std::endl;
        }
    }
    catch (const boost::interprocess::interprocess_exception &e)
    {
        std::cerr << "Error updating shared memory vector: " << e.what() << std::endl;
        throw;
    }

    return false;
}

/**
 * @brief Fetch a value from the shared memory vector at a specific location.
 */
double SMemory::fetchVector(std::string name, unsigned int location)
{
    if (!checkMemory(name))
        return 0;

    ipc::managed_shared_memory managed_shm{ipc::open_only, name.c_str()};
    using vec_t = ipc::vector<double, ipc::allocator<double, ipc::managed_shared_memory::segment_manager>>;

    std::pair<vec_t *, std::size_t> p = managed_shm.find<vec_t>("SharedVector");
    if (p.first)
    {
        std::pair<ipc::interprocess_mutex *, std::size_t> mutex_pair = managed_shm.find<ipc::interprocess_mutex>("SharedMutex");
        if (mutex_pair.first)
        {
            ipc::scoped_lock<ipc::interprocess_mutex> lock(*mutex_pair.first);

            if (location < p.first->size())
            {
                return (*p.first)[location];
            }
        }
    }

    return 0;
}

SMemory::SMemory()
{
    create("latency", IMU_ARRAY_SIZE);
}
