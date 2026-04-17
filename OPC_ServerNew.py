#!/usr/bin/env python
# opcua_server1.py - Create an OPC UA server and simulate 2 tags
#
import opcua
from opcua import ua
import random
import time
import logging
import datetime
import socket
import struct

logging.basicConfig()
logger = logging.getLogger('logger')
logger.warning('The System may break down')

s = opcua.Server()
s.set_server_name("OpcUa Test Server")
s.set_endpoint("opc.tcp://192.168.7.2:4841")

# Register the OPC-UA namespace
idx = s.register_namespace("http://192.168.7.2:4841")
# start the OPC UA server (no tags at this point)
s.start()
timestamp = datetime.datetime.now()

objects = s.get_objects_node()
# Define a Weather Station object with some tags
myobject = objects.add_object(idx, "Station")

# Add a Temperature tag with a value and range
myvar1 = myobject.add_variable(idx, "Temperature", 25)
myvar1.set_writable(writable=True)

# Add a Windspeed tag with a value and range
myvar2 = myobject.add_variable(idx, "Windspeed", 11)
myvar2.set_writable(writable=True)
# Add a Temperature tag with a value and range
myvar3 = myobject.add_variable(idx, "Temperature2", 25)
myvar3.set_writable(writable=True)

# Add a Windspeed tag with a value and range
myvar4 = myobject.add_variable(idx, "Windspeed2", 11)
myvar4.set_writable(writable=True)

# Add a Temperature tag with a value and range
myvar5 = myobject.add_variable(idx, "Temperature3", 25)
myvar5.set_writable(writable=True)

# Add a Windspeed tag with a value and range
myvar6 = myobject.add_variable(idx, "Windspeed3", 11)
myvar6.set_writable(writable=True)
# Add a Temperature tag with a value and range
myvar7 = myobject.add_variable(idx, "Temperature4", 25)
myvar7.set_writable(writable=True)

# Add a Windspeed tag with a value and range
myvar8 = myobject.add_variable(idx, "Windspeed4", 11)
myvar8.set_writable(writable=True)

# Create a socket object
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Define the host and port for the socket connection
host = 'localhost'  # Use your desired host address
port = 8081

# Connect to the C program socket
sock.connect((host, port))

# Create some simulated data
while True:
   
   for i in range(9):
                if i == 0:
                        Dataarray = [None] * 8
                        results = [None] * 8
                else: 

                        # Generate simulated data
                        #temperature = random.randrange(125, 200)
                        #windspeed = random.randrange(200, 700)
                        data = sock.recv(2)  # Assuming 16 bits (2 bytes) of data
                        print("Received data:", data)
                        # Convert the received data to an integer
                        received_data = struct.unpack('<H', data)[0]  # Assuming big-endian byte order
                        #float_value = struct.unpack('!f', struct.pack('!H', received_data))[0]
                        # Set the OPC UA variables with the simulated data
                        Dataarray[i-1] = received_data & 0x0FFF
                        results[i-1] = (received_data & 0xF000) >> (13) 
                        time.sleep(0.1)


   

   for i in range(8):
             if isinstance(Dataarray[i], str) or isinstance(results[i], str):
              print("string bad data")
             else:
              
              print("Received data:", results[i])

              print("Daraarray(1)", Dataarray[i])
              float_value = (Dataarray[i] * 3.433) / 1024
              var = locals()["myvar" + str(results[i]+1)]
              
              var.set_value(float_value)
              


    
    
    #float_value = (received_data * 3.433) / 1024
    
    
    #myvar1.set_value(float_value*100)
    
    #myvar2.set_value(float_value)
    #print("Received data:", received_data)
    # Pack the data into a 16-bit format
    
    # Send the data through the socket connection
    #sock.sendall(data)

    

# Close the socket connection
sock.close()


