
import latency_shared_memory_module
import time

# Get shared values from C++ functions
start_time=time.time()
for i in range(100000):
    shared_int = latency_shared_memory_module.get_shared_int()
    shared_string = latency_shared_memory_module.get_shared_string()
    print(shared_int,shared_string)
    time.sleep(0.5)

end_time=time.time()
elapsed_time=end_time-start_time
print(f"Reading in python elapsed time : {elapsed_time}")

