import struct
from multiprocessing import shared_memory
from ctypes import sizeof, c_double, c_char
import time

MUTEX_SIZE = 40  

def update(shm_name: str, index: int, value: float, size: int):
    try:
        shm = shared_memory.SharedMemory(name=shm_name)
        
        data_offset = MUTEX_SIZE + index * sizeof(c_double)
        
        shm.buf[data_offset:data_offset + sizeof(c_double)] = struct.pack('d', value)
        print(f"[Write] Updated index {index} with value {value}")

        shm.close()
    except FileNotFoundError:
        print("Error: Shared memory does not exist.")
    except Exception as e:
        print(f"Error: {e}")

def fetch(shm_name: str, index: int) -> float:
    try:
        shm = shared_memory.SharedMemory(name=shm_name)
        
        data_offset = MUTEX_SIZE + index * sizeof(c_double)
        
        value = struct.unpack('d', shm.buf[data_offset:data_offset + sizeof(c_double)])[0]
        print(f"[Read] Value at index {index} is {value}")

        shm.close()
        return value
    except FileNotFoundError:
        print("Error: Shared memory does not exist.")
        return 0
    except Exception as e:
        print(f"Error: {e}")
        return 0

if __name__ == "__main__":
    shm_name = "IMU"  
    data_size = 16  
    now_seconds = time.time()
    time_in_us = int(now_seconds * 1_000_000)
    # update(shm_name, index_to_write, value_to_write, data_size)
    read_value = fetch(shm_name, 1)
    oldtime = fetch("latencyPOS", 1)
    print(f"Local: {time_in_us} fetched: {oldtime}")
    latency = oldtime - time_in_us
    print(f"IMU Heading: {read_value}, Latency in microseconds: {latency} µs")