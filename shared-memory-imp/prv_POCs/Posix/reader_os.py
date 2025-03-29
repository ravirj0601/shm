import mmap
import struct
import os

SHARED_MEMORY_NAME = "/PosixSharedMemory"
SIZE = 4096  # Size must match what was set in the C++ program

# Open the shared memory file
shm_fd = os.open("/dev/shm" + SHARED_MEMORY_NAME, os.O_RDONLY)

# Memory map the shared memory
with mmap.mmap(shm_fd, SIZE, mmap.MAP_SHARED, mmap.PROT_READ) as mm:

    offset = 0

    # --- Read integer (4 bytes) ---
    int_value = struct.unpack_from('i', mm, offset)[0]
    print(f"Read integer: {int_value}")
    offset += struct.calcsize('i')  # Move the offset by the size of an int

    # --- Read float (4 bytes) ---
    float_value = struct.unpack_from('f', mm, offset)[0]
    print(f"Read float: {float_value}")
    offset += struct.calcsize('f')  # Move the offset by the size of a float

    # --- Read bool (1 byte) ---
    bool_value = struct.unpack_from('?', mm, offset)[0]
    print(f"Read bool: {bool_value}")
    offset += struct.calcsize('?')  # Move the offset by the size of a bool

    # --- Read char (1 byte) ---
    char_value = struct.unpack_from('c', mm, offset)[0].decode('utf-8')
    print(f"Read char: {char_value}")
    offset += struct.calcsize('c')  # Move the offset by the size of a char

    # --- Read char array (5 characters, null-terminated string) ---
    char_array_size = 6  # We know the length of the array in advance (5 characters + null terminator)
    char_array_value = struct.unpack_from(f'{char_array_size}s', mm, offset)[0].decode('utf-8').rstrip('\x00')
    print(f"Read char array: {char_array_value}")
    offset += struct.calcsize(f'{char_array_size}s')  # Move the offset by the size of the char array

# Close the shared memory file descriptor
os.close(shm_fd)
