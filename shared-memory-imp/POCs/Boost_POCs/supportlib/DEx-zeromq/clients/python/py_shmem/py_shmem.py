#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# '''===================================================================================================================
# @Project : iDEX-DISC1 - SharedMemory API   Filename: shmem_client.py
# @Version : 1.0
# @Date    : 20-Sept-2023
# @Updated : 20-Sept-2023
# @Author  : Keshav Choudhary <keshav.c@planystech.com>
# '''===================================================================================================================
"""This class provides a SharedMemory Client which is meant to fetch and update
 shared memory values provided that the sharedmemory server (zeromq based) is already running."""
# '''===================================================================================================================
import zmq
import struct


class SharedMemoryClient:

    def __init__(self, server_ip="10.0.1.17", server_port="5555"):
        self._message = None
        self._context = zmq.Context()
        self._socket = self._context.socket(zmq.REQ)
        self._connection = None
        try:
            self._connection = self._socket.connect(f"tcp://{server_ip}:{server_port}")
        except (Exception,) as e:
            print(f"ExceptionOccurred: Unable to connect due to: {e}")
            self._connection = None

    def fetch(self, sensor_name, index):
        # TODO: send_strings returns frame or None handle accordingly
        if self._connection is not None:
            try:
                self._message = f"R,{sensor_name.upper()},{index}"
                self._socket.send_string(self._message)
                recv_msg = self._socket.recv()
                value = struct.unpack('d', recv_msg)[0]
                return value
            except (Exception,) as e:
                print(f"ExceptionOccurred: Unable to fetch shm due to: {e}")
                return None
        else:
            print("Cannot 'fetch' as there is no connection with SharedMemoryServer.")
            return None
    
    def fetch_bool(self, sensor_name, index):
        # TODO: send_strings returns frame or None handle accordingly
        if self._connection is not None:
            try:
                self._message = f"B,{sensor_name},{index}"
                self._socket.send_string(self._message)
                recv_msg = self._socket.recv()
                value = struct.unpack('?', recv_msg)[0]
                return value
            except (Exception,) as e:
                print(f"ExceptionOccurred: Unable to fetch shm due to: {e}")
                return None
    
    def fetch_str(self, sensor_name, index):
        # TODO: send_strings returns frame or None handle accordingly
        if self._connection is not None:
            try:
                self._message = f"S,{sensor_name},{index}"
                self._socket.send_string(self._message)
                recv_msg = self._socket.recv()
                value = recv_msg.decode('utf-8')
                return value
            except (Exception,) as e:
                print(f"ExceptionOccurred: Unable to fetch string from shm due to: {e}")
                return None

    def update(self, sensor_name, index, value):
        # TODO: We can check whether the update was made successfully
        #       accordingly return 0 or 1.
        if self._connection is not None:
            try:
                self._message = f"W,{sensor_name.upper()},{index},{value}"
                self._socket.send_string(self._message)
                recv_msg = self._socket.recv()
                value = struct.unpack('d', recv_msg)[0]
                return 0
            except (Exception,) as e:
                print(f"ExceptionOccurred: Unable to update shm due to: {e}")
                return 1
        else:
            print("Cannot 'update' as there is no connection with SharedMemoryServer.")
            return 1

    def update_bool(self, sensor_name, index, value):
        # TODO: We can check whether the update was made successfully
        #       accordingly return 0 or 1.
        if self._connection is not None:
            try:
                self._message = f"B,{sensor_name.upper()},{index},{value}"
                self._socket.send_string(self._message)
                recv_msg = self._socket.recv()
                value = struct.unpack('?', recv_msg)[0]
                return value
            except (Exception,) as e:
                print(f"ExceptionOccurred: Unable to update shm due to: {e}")
                return 1
        else:
            print("Cannot 'update' as there is no connection with SharedMemoryServer.")
            return 1

