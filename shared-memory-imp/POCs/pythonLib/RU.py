import shm_test

shm = shm_test.SMemory()
while(20):
    heading = shm.fetch("IMU", 1)
    pitch = shm.fetch("IMU", 2)
    roll = shm.fetch("IMU", 3)
    depth = shm.fetch("PressureSensor", 1)
    
    print(f"IMU heading:: {heading}")
    print(f"IMU pitch:: {pitch}")
    print(f"IMU roll:: {roll}") 
    print(f"PS_depth:: {depth}")