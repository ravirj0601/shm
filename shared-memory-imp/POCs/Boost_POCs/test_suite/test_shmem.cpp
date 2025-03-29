#include "test_shmem.h"

using namespace boost::interprocess;

int main()
{
	/* For randomization */
	srand(time(NULL));

	/* Local variables */
	// int count = 0;
	double PressureSensor_depth = 0.0;
	double IMU_roll = 0.0, IMU_pitch = 0.0, IMU_heading = 0.0;
	double target_depth = 0.0;
	double target_pitch = 0.0;
	double sway_fore_speed = 0.0;
	double sway_aft_speed = 0.0;
	double surge_starboard_speed = 0.0;
	double surge_port_speed = 0.0;
	std::string sensor_data = "";
	static SMemory s_memory;


	/* Initialization */
	// SMemory s_memory;

	// while (count < 10) {
	while (true) {
		IMU_heading = (float)rand() / 1000;
		IMU_pitch = (float)rand() / 10;
		IMU_roll = (float)rand() / 2;
		PressureSensor_depth = (float)rand() / 200;
		for (unsigned i = 0; i < 16; i++)
			s_memory.update("HMU", (i + 1), (double)rand()/100000, pow(2, 4));

		s_memory.update("IMU", 1, IMU_heading, 5);
		s_memory.update("IMU", 2, IMU_pitch, 5);
		s_memory.update("IMU", 3, IMU_roll, 5);

		std::cout << "IMU heading:: orig: " << IMU_heading << ", shm: " << s_memory.fetch("IMU", 1) << "\n";
		std::cout << "IMU pitch:: orig: " << IMU_pitch << ", shm: " << s_memory.fetch("IMU", 2) << "\n";
		std::cout << "IMU roll:: orig: " << IMU_roll << ", shm: " << s_memory.fetch("IMU", 3) << "\n";
		std::cout << "PS_depth: orig: " << PressureSensor_depth << ", shm: " << s_memory.fetch("PressureSensor", 1) << "\n";

		target_depth = s_memory.fetch("SetPoints", 1);
		target_pitch = s_memory.fetch("SetPoints", 2);
		sway_fore_speed = s_memory.fetch("SetPoints", 3);
		sway_aft_speed = s_memory.fetch("SetPoints", 4);
		surge_starboard_speed = s_memory.fetch("SetPoints", 5);
		surge_port_speed = s_memory.fetch("SetPoints", 6);

		sensor_data = s_memory.fetch_all("HMU");

		std::cout << "target_depth: " << target_depth << ", target_pitch: " << target_pitch << ", sway_fore_speed: " << sway_fore_speed << \
			", sway_aft_speed: " << sway_aft_speed << ", surge_starboard_speed: " << surge_starboard_speed << ", surge_port_speed: " << surge_port_speed << "\n";
		std::cout << "[IMU] Roll: " << s_memory.fetch("IMU", 1) << ", Pitch: " << s_memory.fetch("IMU", 2) << ", Heading: " << s_memory.fetch("IMU", 3) << "\n";
		std::cout << "[SetPoints] target_depth: " << s_memory.fetch("SETPOINTS", 1) << ", target_pitch: " << s_memory.fetch("SETPOINTS", 2) << \
			", sway fore PWM: " << s_memory.fetch("SETPOINTS", 3) << ", sway aft PWM: " << s_memory.fetch("SETPOINTS", 4) << \
			", surge starboard PWM: " << s_memory.fetch("SetPoints", 5) << ", surge port PWM: " << s_memory.fetch("SetPoints", 6) << "\n";

		std::cout << "[HMU] " << sensor_data << "\n";
		sensor_data.clear();
		// count++;
		sleep(1);
	}

	/* -ve usecases */
	std::cout << "Mysmv[1]: " << s_memory.fetch("Mysmv", 1) << "\n";	// Variable name doesn't exist
	std::cout << "Imu[3]: " << s_memory.fetch("Imu", 3) << "\n";		// Variable case do not match
	std::cout << "IMU[9]; " << s_memory.fetch("IMU", 9) << "\n";		// Variable index not available

	return 0;
}
