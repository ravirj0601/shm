## Compiler flags
CC = g++
CFLAGS = -g -Wall -O2 -std=c++11 -s
LDFLAGS = -lrt -lboost_system -lpthread

## Generic rule
all:	test_shmem


## Individual rules
test_shmem:	test_shmem.cpp
	$(CC) $(CFLAGS) test_shmem.cpp ../shmem_wrapper.cpp -o test_shmem $(LDFLAGS)

## Clean-up ops
clean:
	rm -rf	test_shmem \
		test_shmem.o \
		shmem_wrapper.o
