#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H
#define VALUE 0

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cstdbool>
#include <cmath>

class SMemory {
	private:
		// Check if the said shared memory exists
		bool checkMemory(std::string);
		// Create the shared memory variable to relevant details
		void create(std::string name, unsigned int data_size);
	public:
		SMemory();	// Constructor
		// Fetch the value for the shared memory variable
		double fetch(std::string name, unsigned int arr_location);
		// Fetch values of all indices for the shared memory variable
		std::string fetch_all(std::string name);
		// Update the value of the shared memory variable
		bool update(std::string name, unsigned int arr_location, double value, unsigned int array_size);
};

#endif	/* SHARED_MEMORY_H */
