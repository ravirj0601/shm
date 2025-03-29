#include "shmem_wrapper.h"
#include <string>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

using namespace boost::interprocess;

#define DEFAULT_ARRAY_SIZE		pow(2, 4)

#define IMU_ARRAY_SIZE			DEFAULT_ARRAY_SIZE
#define PRESSURESENSOR_ARRAY_SIZE	DEFAULT_ARRAY_SIZE
#define GPS_ARRAY_SIZE			DEFAULT_ARRAY_SIZE
#define DVL_ARRAY_SIZE			DEFAULT_ARRAY_SIZE
#define SETPOINTS_ARRAY_SIZE		DEFAULT_ARRAY_SIZE
#define SMCU_ARRAY_SIZE			DEFAULT_ARRAY_SIZE
#define HMU_ARRAY_SIZE			DEFAULT_ARRAY_SIZE


// Structure that includes the mutex and data
struct SharedData {
    interprocess_mutex mutex;   
    double data[16]; 
};

/**
 * @brief Utility function to check if the shared memory variable exists or not
 */
bool SMemory::checkMemory(std::string variable) {
    std::string path = "/dev/shm/" + variable;
    return access(path.c_str(), F_OK) != -1;
}

/**
 * @brief Utility function to create/allocate shared memory with a mutex for synchronization
 */
void SMemory::create(std::string name, unsigned int data_size) {
    if (checkMemory(name))
        return;

    managed_shared_memory managed_shm{open_or_create, name.c_str(), 1024};
    if (!managed_shm.find<SharedData>("SharedData").first)
        managed_shm.construct<SharedData>("SharedData")();
}

/**
 * @brief Utility function to fetch value of some shared memory variable
 */
double SMemory::fetch(std::string name, unsigned int arr_location) {
    if (!checkMemory(name))
        return 0;

    managed_shared_memory managed_shm{open_only, name.c_str()};
    
    std::pair<SharedData *, std::size_t> p = managed_shm.find<SharedData>("SharedData");
    if (p.first) {
        scoped_lock<interprocess_mutex> lock(p.first->mutex);
        return p.first->data[arr_location - 1]; 
    }

    return 0;
}

/**
 * @brief Utility function to fetch all values from shared memory
 */
std::string SMemory::fetch_all(std::string name) {
    std::string sensor_data = "";
    if (checkMemory(name)) {
        managed_shared_memory managed_shm{open_only, name.c_str()};
        std::pair<SharedData *, std::size_t> p = managed_shm.find<SharedData>("SharedData");

        if (p.first) {
            scoped_lock<interprocess_mutex> lock(p.first->mutex);
            for (unsigned i = 0; i < DEFAULT_ARRAY_SIZE - 1; i++) {
                sensor_data += std::to_string(p.first->data[i]);
                sensor_data += ",";
            }
            sensor_data += std::to_string(p.first->data[15]);
            sensor_data += '\0';
        }
    }
    return sensor_data;
}

/**
 * @brief Utility function to update shared memory variable value
 */
bool SMemory::update(std::string name, unsigned int arr_location,
                     double value, unsigned int array_size) {
    /* Sanity check */
    if (arr_location > array_size)
        return false;

    /* Create the shared memory if it doesn't exist */
    if (!checkMemory(name)) {
        this->create(name, array_size);
    }
    managed_shared_memory  managed_shm{open_only, name.c_str()};
    std::pair<SharedData *, std::size_t> p = managed_shm.find<SharedData>("SharedData");
    if (p.first) {
        scoped_lock<interprocess_mutex> lock(p.first->mutex);
        p.first->data[arr_location - 1] = value;
        return true;
    }

    return false;
}

/**
 * @brief	Constructor. Creates the shared memory variables
 * 		as part of initialization.
 * @param	None
 * @return	None
 */
SMemory::SMemory(void)
{
	create("IMU", IMU_ARRAY_SIZE);
	create("PressureSensor", PRESSURESENSOR_ARRAY_SIZE);
	create("GPS", GPS_ARRAY_SIZE);
	create("DVL", DVL_ARRAY_SIZE);
	create("SetPoints", SETPOINTS_ARRAY_SIZE);
	create("SMCU", SMCU_ARRAY_SIZE);
	create("HMU", HMU_ARRAY_SIZE);
	
}