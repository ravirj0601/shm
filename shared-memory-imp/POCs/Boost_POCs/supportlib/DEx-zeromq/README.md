# What's this ZeroMQ thing?

ZeroMQ is an asynchronous messaging library, aimed at use in distributed or concurrent applications. It provides a message queue, but unlike message-oriented middleware, a ZeroMQ system can run without a dedicated message broker; the zero in the name is for zero broker.

# How to install it?

For Ubuntu based systems, you should be able to install it by simply running the command -

`sudo apt install libzmq3-dev`

# Will this allow me to use `zmq` package in Python out of the box?

No. You'll have to install it through pip3 -

`pip3 install zmq`
