## Introduction
This class provides a SharedMemory Client which is meant to fetch and update shared memory values provided that the SharedMemory Server is already running.

## Required Packages
`pip3 install pyzmq`


## Troubleshooting
1. Client is stuck and not returning any value:
    This behaviour is observed when the client is not able to communicate with the server.
    Check server is running, check server IP and port are correct.
