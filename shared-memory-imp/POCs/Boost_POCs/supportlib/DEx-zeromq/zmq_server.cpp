/**
 *
 * @brief	ZeroMQ server utility code. Should be run through
 * 		SystemD as a background process
 *
 * @date	20-09-2023
 *
 * @version	1.0
 *
 * @author	Subhra Sankha Sarkar <subhra.s@planystech.com>
 *
 * @note	This is the base draft. This software has considerable
 * 		limitations -
 *
 * 		TODO
 *
 * 		- No checks if certain sensors exist or not
 * 		- No checks for ArrayIndexOutOfBounds
 * 		- Doesn't have exception handling
 * 		- Lacks modularity
 *
 */
#include "zmq_server.h"
#include <cstring>
#include <zmq.hpp>

using namespace boost::interprocess;
using namespace zmq;

int main () {
	//  ZeroMQ: Prepare our context and socket
	zmq::context_t context (2);
	zmq::socket_t socket (context, zmq::socket_type::rep);
	zmq::message_t request;
	zmq::message_t reply;
	socket.bind ("tcp://10.0.1.17:5555");
	
	
	SMemory s_memory;
	
	// Variable declarations
	std::string shmem_msg = "";
	std::string sensor_name = "";
	std::string sensor_param_value = "";
	std::string s_index = "";
	std::string sensor_data = "";
	size_t str_size = 0;
	bool shmem_bool = false;
	
	int pos = -1;
	int pos_comma_first = -1;
	int pos_comma_second = -1;
	int pos_comma_last = -1;
	int index = -1;
	int rw = -1;
	
	double shmem_value = 0.0;
	double write_val = 0.0;

	while (true) {
		request.rebuild();
		reply.rebuild();
		sensor_data.clear();

		//  Wait for next request from client
		socket.recv (request, zmq::recv_flags::none);
        std::cout << "Received " << request << " ";

		// Find the first occurrance of '('
		pos = request.str().find('(');
		
		// Create substr
		shmem_msg = request.str().substr(pos);
		std::cout << shmem_msg << "\n";

		// CSV based parsing
		pos_comma_first = shmem_msg.find(',');
		pos_comma_second = shmem_msg.substr(pos_comma_first+1).find(',') + pos_comma_first + 1;
		pos_comma_last = shmem_msg.find_last_of(',');
		// std::cout << "Ci1: " << pos_comma_first << ", ci2: " << pos_comma_second << ", ci3: " << pos_comma_last << "\n";

		rw = shmem_msg[pos_comma_first - 1];					// (R)ead or (W)rite, taking care of the preceding space char
		sensor_name = shmem_msg.substr((pos_comma_first+1), pos_comma_second - pos_comma_first - 1);
		s_index = shmem_msg.substr((pos_comma_second + 1), pos_comma_last - pos_comma_second - 1);
		try {
			index = stoi(s_index);
		} catch (std::exception &e) {
			std::cout << "[Error] Failed to convert string to integer: " << e.what() << "\n";
		}
		sensor_param_value = shmem_msg.substr(pos_comma_last + 1);
		sensor_param_value.pop_back();						// Remove the trailing ')' char
		try {
			write_val = std::stod(sensor_param_value);			
		} catch (std::exception &e) {
			std::cout << "[Error] Failed to convert string to double: " << e.what() << "\n";
		}
		std::cout << "Read/Write: " << rw << ", Sensor: " << sensor_name << ", index: " << index << ", value: " << write_val << "\n";
		
		// Read or update the shared memory variable
		if (rw == 'R') {
			if (index == 42) {			// Return all elements of the sensor array, 42 is the ASCII value for '*'
				sensor_data = s_memory.fetch_all(sensor_name);
				std::cout << "[" << sensor_name << "] " << sensor_data << "\n";
			} else if (index > 0 && index <= 16) {	// Return data for that specific index
				shmem_value = s_memory.fetch(sensor_name, double(index));
			}
		} else if (rw == 'W') {	  // If we're updating the shared memory variable value
        		// Now that array sizes have been changed to 16 from 8, had to make this fix
			s_memory.update(sensor_name, index, write_val, 16);
			shmem_value = s_memory.fetch(sensor_name, index);
		} else if(rw == 'B') {
			if (index == 66) {  // Return Boolean value, 66 is the ASCII value for 'B'
                        	shmem_bool = s_memory.fetch_bool(sensor_name);
				std::cout << "[" << sensor_name << "] " << shmem_bool << "\n";	
			} else if (index == 67) {
				s_memory.update_bool(sensor_name, write_val);
			}
		} else if (rw == 'S') {
			if (index == 83) {   // Return String value, 83 is the ASCII value for 'S'
                        	sensor_data = s_memory.fetch_str(sensor_name);
				std::cout << "[" << sensor_name << "] " << sensor_data << "\n";
			}
		} else {
			std::cout << "Your request could not be handled\n";
		}
		// std::cout << "Smem value: " << shmem_value << "\n";


        //  Do some 'work'
		usleep(10);

		if (index == 42) {    // For returning all the data pertaining to a sensor
			str_size = sensor_data.length();
			reply.rebuild(str_size);
			std::memcpy(reply.data(), sensor_data.c_str(), str_size);
		} else if (index > 0 && index <= 16) {
			reply.rebuild(8);
			std::memcpy(reply.data(), &shmem_value, 8);
		} else if (index == 66) {
			reply.rebuild(1);
			std::memcpy(reply.data(), &shmem_bool, 1);
		} else if (index == 83) {
			str_size = sensor_data.length();
			reply.rebuild(str_size);
			std::memcpy(reply.data(), sensor_data.c_str(), str_size);
		}


#if 0
        	//  Send reply back to client
        	// zmq::message_t reply (str_size + 10);
		// Modify this to make it more understandable
		if (num_elements == 1)
			std::memcpy(reply.data (), &shmem_value, 8);
		else {
			std::memcpy(reply.data (), sensor_data.c_str(), (str_size + 10));
		}
		// num_elements == 1 ? std::memcpy(reply.data (), &shmem_value, (8 * num_elements)) : std::memcpy(reply.data (), &d_arr, (8 * num_elements));
$		// std::memcpy(reply.data (), &shmem_value, (8 * num_elements));
#endif
        	socket.send (reply, zmq::send_flags::none);
    	}
    	return 0;
}
