
import sysv_ipc
import struct
import time

shm = sysv_ipc.SharedMemory(101)

start_time = time.time()
for i in range(100000):
    data = shm.read()
    unpacked_data = struct.unpack('i256s', data)
    id_value = unpacked_data[0]
    name_value = unpacked_data[1].decode('utf-8').strip('\x00')  # Remove null bytes
    
    print(f"SystemV: {id_value}")
    print(f"SystemV: {name_value}")
    time.sleep(0.5)

end_time = time.time()
elapsed_time=end_time-start_time
print(f"Reading from python Elapsed time: {elapsed_time}")

