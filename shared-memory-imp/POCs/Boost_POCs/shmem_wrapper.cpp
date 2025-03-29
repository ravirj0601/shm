#include "shmem_wrapper.h"
#include <string>

using namespace boost::interprocess;

#define DEFAULT_ARRAY_SIZE		pow(2, 4)

#define IMU_ARRAY_SIZE			DEFAULT_ARRAY_SIZE
#define PRESSURESENSOR_ARRAY_SIZE	DEFAULT_ARRAY_SIZE
#define GPS_ARRAY_SIZE			DEFAULT_ARRAY_SIZE
#define DVL_ARRAY_SIZE			DEFAULT_ARRAY_SIZE
#define SETPOINTS_ARRAY_SIZE		DEFAULT_ARRAY_SIZE
#define SMCU_ARRAY_SIZE			DEFAULT_ARRAY_SIZE
#define HMU_ARRAY_SIZE			DEFAULT_ARRAY_SIZE
#define FLAG_MONITOR_ARRAY_SIZE		DEFAULT_ARRAY_SIZE
#define PLATFORM_GPS_RF_SIZE		DEFAULT_ARRAY_SIZE
#define CM_GPS_RF_ARRAY_SIZE		DEFAULT_ARRAY_SIZE
#define VISUALISER_ARRAY_SIZE		DEFAULT_ARRAY_SIZE

/**
 * @brief	Utility function to check if the shared memory variable
 * 		exists or not
 * @param1	variable - Shared memory variable name
 * @return	bool	True - If such named shared memory region is found
 * 			False - Otherwise
 */
bool SMemory::checkMemory(std::string variable)
{
	std::string path = "/dev/shm/" + variable;
	if (access(path.c_str(), F_OK) == -1)
		return false;
	return true;
}

/**
 * @brief	Utility function to create/allocate shared memory chunks of
 * 		size 1024 bytes for storing double values
 * @param1	name - Name of the shared memory variable
 * @param2	data_size - Array size
 * @return	None
 */
void SMemory::create(std::string name, unsigned int data_size)
{
	if (checkMemory(name))
		return;

	managed_shared_memory managed_shm{open_or_create, name.c_str(), 1024};
	if (!managed_shm.find<double>("double").first)
		managed_shm.construct<double>("double")[data_size](0.0);
}

/**
 * @brief	Utility function to fetch value of some shared memory
 * 		variable
 * @param1	name - Shared memory variable name
 * @param2	arr_location - Index in the array
 * @return	double value
 */
double SMemory::fetch(std::string name, unsigned int arr_location)
{
	if (!checkMemory(name))
		return 0; 

	managed_shared_memory managed_shm{open_only, name.c_str()};
	std::pair<double *, std::size_t> p = managed_shm.find<double>("double");
	if (p.first)
		return *(p.first + arr_location - 1);

	return 0;
}

/**
 * @brief	Utility function to fetch values from all indices of some shared
 * 		memory variable
 * @param1	name - Shared memory variable name
 * @return	string dump
 */
std::string SMemory::fetch_all(std::string name)
{
	std::string sensor_data = "";
	if (checkMemory(name)) {
		managed_shared_memory managed_shm{open_only, name.c_str()};
		std::pair<double *, std::size_t> p = managed_shm.find<double>("double");
		if (p.first) {
			for (unsigned i = 0; i < DEFAULT_ARRAY_SIZE - 1 ; i++) {
				sensor_data += std::to_string(*(p.first + i));
				sensor_data += ",";
			}
			sensor_data += std::to_string(*(p.first + 15));
			sensor_data += '\0';
		}
	}
	return sensor_data;
}

/**
 * @brief	Utility function to update shared memory variable value(s)
 * @param1	name - Name of the shared memory variable
 * @param2	arr_location - 1 based index in the array
 * @param3	value - The double value to be assigned
 * @param4	array_size - Size of the array
 * @return	bool - 	True if able to update
 * 			False otherwise
 */
bool SMemory::update(std::string name, unsigned int arr_location,
                     double value, unsigned int array_size)
{
	/* Sanity check */
	if (arr_location > array_size)
		return false;

	/* If the shmem chunk hasn't been created yet */
	if (!checkMemory(name))
	{
		this->create(name, array_size);
#if 0		// We shoudn't need the following 2 lines. Check it.
		for(int i = 1; i <= array_size; i++)
			this->update(name, i, 0, array_size);
#endif
	}

	/* Now that it's created (or existing from before), update it */
	managed_shared_memory managed_shm{open_only, name.c_str()};
	double *i = managed_shm.find_or_construct<double>("double")();
	*(i + arr_location - 1) = value;
	return true;
}

/**
 * @brief	Constructor. Creates the shared memory variables
 * 		as part of initialization.
 * @param	None
 * @return	None
 */
SMemory::SMemory(void)
{
	/* Create shared memory for IMU */
	create("IMU", IMU_ARRAY_SIZE);
	/* Create shared memory for PressureSensor */
	create("PressureSensor", PRESSURESENSOR_ARRAY_SIZE);
	/* Create shared memory for GPS */
	create("GPS", GPS_ARRAY_SIZE);
	/* Create shared memory for DVL */
	create("DVL", DVL_ARRAY_SIZE);
	/* Create shared memory for Set Points */
	create("SetPoints", SETPOINTS_ARRAY_SIZE);
	/* Create shared memory for SMCU */
	create("SMCU", SMCU_ARRAY_SIZE);
	/* Create shared memory for HMU */
	create("HMU", HMU_ARRAY_SIZE);
	/* Create shared memory for Process Flag Monitor */
	create("Flag_Monitor", FLAG_MONITOR_ARRAY_SIZE);
	/* Create shared memory for Platform GPS */
	create("Platform_GPS_RF", PLATFORM_GPS_RF_SIZE);
	/* Create shared memory for CM GPS RF */
	create("CM_GPS_RF", CM_GPS_RF_ARRAY_SIZE);
	/* Create shared memory for Visualiser */
	create("Visualiser", VISUALISER_ARRAY_SIZE);
}
