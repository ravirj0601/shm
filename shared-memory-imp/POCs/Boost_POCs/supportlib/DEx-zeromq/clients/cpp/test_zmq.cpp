#include <zmq.hpp>
#include <string>
#include <iostream>
#include <unistd.h>

int main ()
{
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, zmq::socket_type::req);
    zmq::message_t request;
    zmq::message_t reply;

    std::cout << "Connecting to hello world server..." << std::endl;
    socket.connect ("tcp://127.0.0.1:5555");

    int count = 0;

	while (true) {
		request.rebuild(32);
		reply.rebuild(32);
		++count;
		if (count % 2) {
			memcpy (request.data (), "R,HMU,42,123.456", sizeof("R,HMU,42,123.456"));
			std::cout << "Sending 'R,HMU,42,123.456'" << std::endl;
			socket.send (request, zmq::send_flags::none);
			
			//  Get the reply.
			zmq::message_t reply;
			socket.recv (reply, zmq::recv_flags::none);
			std::cout << "Received " << reply << std::endl;
		} else {
			memcpy (request.data (), "R,IMU,2,123.456", sizeof("R,IMU,2,123.456"));
			std::cout << "Sending 'R,IMU,2,123.456'" << std::endl;
			socket.send (request, zmq::send_flags::none);
			
			//  Get the reply - double converted to hex.
			socket.recv (reply, zmq::recv_flags::none);
			std::cout << "Received " << reply << std::endl;
		}
		sleep(1);
	}
    return 0;
}
