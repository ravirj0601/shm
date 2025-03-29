import sysv_ipc
import numpy as np
import os
import errno
import time

class SharedMemoryManager:
    def __init__(self):
        self.memory_size = 1024  
        self.sem_key = 5678  
        self.shm_key = 1234  
        self.max_size = self.memory_size  

    def generate_key(self, shm_name):
        # return sysv_ipc.ftok(shm_name, 65)
        return hash(shm_name + str(os.getpid()) + str(time.time()))
    def open_or_create_shm(self, shm_name, size):
        key = self.generate_key(shm_name)
        try:
            shm = sysv_ipc.SharedMemory(key, sysv_ipc.IPC_CREAT, size=size * np.dtype(np.float64).itemsize)
            print(f"Shared memory segment created or opened with ID: {shm.id}")
            return shm
        except sysv_ipc.ExistentialError:
            print("Shared memory segment already exists.")
            return sysv_ipc.SharedMemory(key)

    def initialize_semaphore(self, sem_id):
        sem = sysv_ipc.Semaphore(self.sem_key, sysv_ipc.IPC_CREAT, initial_value=1)
        print(f"Semaphore initialized with ID: {sem.id}")
        return sem

    def lock_semaphore(self, sem):
        sem.acquire()

    def unlock_semaphore(self, sem):
        sem.release()

    def write(self, shm_name, index, value, size):
        shm = self.open_or_create_shm(shm_name, size)
        sem = self.initialize_semaphore(self.sem_key)

        with shm:
            data = np.frombuffer(shm.read(), dtype=np.float64)
            if index >= size:
                print("Error: Index out of bounds.")
                return

            self.lock_semaphore(sem)

            data[index] = value
            shm.write(data.tobytes())  

            self.unlock_semaphore(sem)

        print(f"Value {value} written at index {index}")

    def read(self, shm_name, index):
        key = self.generate_key(shm_name)
        try:
            shm = sysv_ipc.SharedMemory(key)
            with shm:
                data = np.frombuffer(shm.read(), dtype=np.float64)
                if index < self.memory_size:
                    return data[index]
                else:
                    print("Error: Index out of bounds.")
                    return None
        except sysv_ipc.ExistentialError:
            print("[READ] Error: Shared memory does not exist.")
            return None

    def enqueue(self, shm_name, value, max_size):
        shm = self.open_or_create_shm(shm_name, max_size)
        sem = self.initialize_semaphore(self.sem_key)

        with shm:
            data = np.frombuffer(shm.read(), dtype=np.float64)
            front = int(data[-2])  
            rear = int(data[-1])  

            self.lock_semaphore(sem)

            if (rear + 1) % max_size == front:
                print("Error: Queue is full.")
            else:
                
                data[rear] = value
                data[-1] = (rear + 1) % max_size  
                shm.write(data.tobytes())

            self.unlock_semaphore(sem)

        print(f"Enqueued value {value}")

    def dequeue(self, shm_name, max_size):
        key = self.generate_key(shm_name)
        try:
            shm = sysv_ipc.SharedMemory(key)
            with shm:
                data = np.frombuffer(shm.read(), dtype=np.float64)
                front = int(data[-2]) 
                rear = int(data[-1])  

                sem = self.initialize_semaphore(self.sem_key)
                self.lock_semaphore(sem)

                if front == rear:
                    print("Error: Queue is empty.")
                    self.unlock_semaphore(sem)
                    return None
                else:
                    value = data[front]
                    data[-2] = (front + 1) % max_size  
                    shm.write(data.tobytes())

                self.unlock_semaphore(sem)

            print(f"Dequeued value {value}")
            return value
        except sysv_ipc.ExistentialError:
            print("[DEQUEUE] Error: Shared memory does not exist.")
            return None

    def close_shm(self, shm_name):
        key = self.generate_key(shm_name)
        try:
            shm = sysv_ipc.SharedMemory(key)
            sem = sysv_ipc.Semaphore(self.sem_key)
            sem.remove()  
            shm.remove()  
            print(f"Shared memory and semaphore removed for {shm_name}.")
            return True
        except sysv_ipc.ExistentialError:
            print("Error: Shared memory or semaphore does not exist.")
            return False


if __name__ == "__main__":
    shm_manager = SharedMemoryManager()

    shm_name = "/test_shm"
    shm_name1 = "/qtest_shm"

    value = shm_manager.read(shm_name, 0)
    print(f"Value at index 0: {value}")

    dequeued_value = shm_manager.dequeue(shm_name1, 1024)
    print(f"Dequeued value: {dequeued_value}")

    shm_manager.close_shm(shm_name)
