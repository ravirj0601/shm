## Compiler flags
CC = g++
CFLAGS = -g -Wall -O2 -std=c++11 -s
LDFLAGS = -lzmq -lrt -lboost_system -lpthread

## Generic rule
all:	zmq_server


## Individual rules
zmq_server:	zmq_server.cpp
	$(CC) $(CFLAGS) zmq_server.cpp ../SharedMemoryWrapper/shmem_wrapper.cpp -o zmq_server $(LDFLAGS)

## Clean-up ops
clean:
	rm -rf	zmq_server \
		zmq_server.o
