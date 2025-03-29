import shared_memory_module

# Get shared values from C++ functions
shared_int = shared_memory_module.get_shared_int()
shared_bool = shared_memory_module.get_shared_bool()
shared_double = shared_memory_module.get_shared_double()
shared_string = shared_memory_module.get_shared_string()
shared_vector = shared_memory_module.get_shared_vector()

print(shared_int, shared_bool, shared_double, shared_string, len(shared_vector),shared_vector[0])
