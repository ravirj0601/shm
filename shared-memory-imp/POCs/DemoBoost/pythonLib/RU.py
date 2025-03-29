import shm_test
import time
import random

shm = shm_test.SMemory()
while(20):
    heading = random.random()
    # shm.fetch("IMU", 1)
    # pitch = shm.fetch("IMU", 2)
    # roll = shm.fetch("IMU", 3)
    depth = shm.fetch("DEVICE2", 1)
    # ps = shm.update("IMU",1,heading, 4)
    # print(f"{ps}")
    print(f"DEVICE1: {depth}")
    # print(f"IMU pitch:: {pitch}")
    # print(f"IMU roll:: {roll}") 
    # print(f"PS_depth:: {depth}")
    
    time.sleep(0.1)