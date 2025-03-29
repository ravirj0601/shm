
import sysv_ipc
import struct

def GetSharedMemoryAddressInExtractedForm(key,format):
    shm = sysv_ipc.SharedMemory(key)
    data = shm.read()
    unpacked_data = struct.unpack(format, data)
    #shm.remove()
    return unpacked_data


unpacked_data=GetSharedMemoryAddressInExtractedForm(65,'i256s')
id_value = unpacked_data[0]
name_value = unpacked_data[1].decode('utf-8').strip('\x00')  # Remove null bytes
print(f"StructureID: {id_value}")
print(f"StructureName: {name_value}")




unpacked_data=GetSharedMemoryAddressInExtractedForm(66,'i')
id_value = unpacked_data[0]
print(f"Int: {id_value}")



unpacked_data=GetSharedMemoryAddressInExtractedForm(67,'5i')
id_value1 = unpacked_data[0]
id_value2 = unpacked_data[1]
id_value3 = unpacked_data[2]
id_value4 = unpacked_data[3]
id_value5 = unpacked_data[4]
print(f"ArrayInt: {id_value1}")
print(f"ArrayInt: {id_value2}")
print(f"ArrayInt: {id_value3}")
print(f"ArrayInt: {id_value4}")
print(f"ArrayInt: {id_value5}")



unpacked_data=GetSharedMemoryAddressInExtractedForm(68,'5c')
id_value1 = unpacked_data[0].decode('utf-8').rstrip('\x00')
id_value2 = unpacked_data[1].decode('utf-8').rstrip('\x00')
id_value3 = unpacked_data[2].decode('utf-8').rstrip('\x00')
id_value4 = unpacked_data[3].decode('utf-8').rstrip('\x00')
id_value5 = unpacked_data[4].decode('utf-8').rstrip('\x00')
print(f"ArrayChar: {id_value1}")
print(f"ArrayChar: {id_value2}")
print(f"ArrayChar: {id_value3}")
print(f"ArrayChar: {id_value4}")
print(f"ArrayChar: {id_value5}")



