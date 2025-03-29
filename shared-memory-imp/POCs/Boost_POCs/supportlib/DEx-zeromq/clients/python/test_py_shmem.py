#!/usr/bin/env python3
from py_shmem.py_shmem import SharedMemoryClient
import time

def main():
    # Prerequisite for using shared memory 
    shmem_c = SharedMemoryClient()
    # To update the shared memory variable
    # param1	sensor name (case-sensitive)
    # param2	index (1 based)
    # param3 	value you want to update it to
    # ret = shmem_c.update("IMU", 1, 101011)
    
    # To fetch the value from shared memory variable
    # param1	sensor name (case-sensitive)
    # param2	index (1 based)
    # value = shmem_c.fetch_bool("Vision_storing_flag", 66)
    # path = shmem_c.fetch_str("Vision_storing_path", 83)
    # imu_val = shmem_c.fetch("IMU", 1)
    shmem_c.update_bool("VideoStore", 67, True)
    # print(f"Vision Storing flag: {value}")
    # print(f"Vision Storing path: {path}")
    # print(f"IMU Value: {imu_val}")
    value = shmem_c.fetch_bool("VideoStore", 66)
    print(f"Vision flag: {value}")

if __name__ == "__main__":
	main()
