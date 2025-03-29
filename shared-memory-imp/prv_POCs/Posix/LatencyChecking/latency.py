import posix_ipc
import mmap
import struct
import time

SHARED_MEMORY_NAME = "/LatencyPosixSharedMemory"
SIZE = 4+256  # Size must match the C++ program

# Open the shared memory object
shared_memory = posix_ipc.SharedMemory(SHARED_MEMORY_NAME, posix_ipc.O_RDONLY)

# Memory map the shared memory
with mmap.mmap(shared_memory.fd, SIZE, mmap.MAP_SHARED, mmap.PROT_READ) as mm:

    start_time=time.time()
    for i in range(100000):
        offset = 0
        int_value = struct.unpack_from('i', mm, offset)[0]
        offset += struct.calcsize('i')
        
        char_array_size = 256  
        char_array_value = struct.unpack_from(f'{char_array_size}s', mm, offset)[0].decode('utf-8').rstrip('\x00')
        offset += struct.calcsize(f'{char_array_size}s')  
        
        print(f"{int_value}")
        print(f"{char_array_value}")
        time.sleep(0.5)

    end_time=time.time()
    elapsed_time=end_time-start_time
    print(f"Reading in python elapsed time : {elapsed_time}")