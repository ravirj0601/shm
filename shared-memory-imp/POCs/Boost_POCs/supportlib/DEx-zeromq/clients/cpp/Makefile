## Compiler flags
CC = g++
CFLAGS = -g -Wall -O2 -std=c++11 -s
LDFLAGS = -lzmq

## Generic rule
all:	test_zmq


## Individual rules
test_zmq:	test_zmq.cpp
	$(CC) $(CFLAGS) test_zmq.cpp -o test_zmq $(LDFLAGS)

## Clean-up ops
clean:
	rm -rf	test_zmq \
		test_zmq.o

