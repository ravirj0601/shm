#!/bin/bash

# Compile the writer.cpp file
g++ -I/home/akhilesh/Downloads/boost_1_58_0 latency.cpp -o latency -lrt -pthread
if [ $? -ne 0 ]; then
    echo "Failed to compile writer.cpp"
    exit 1
fi
echo "writer.cpp compiled successfully."

# Compile the shared_memory_module.cpp file as a shared library
g++ -shared -fPIC -I/usr/include/python3.8 -I/home/akhilesh/Downloads/boost_1_58_0 latency_shared_memory_module.cpp -o latency_shared_memory_module.so -lboost_python38 -lrt
if [ $? -ne 0 ]; then
    echo "Failed to compile shared_memory_module.cpp"
    exit 1
fi
echo "shared_memory_module.cpp compiled successfully."

# Output success message
echo "Compilation completed successfully."
