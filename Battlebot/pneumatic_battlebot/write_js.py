# Open the js0 device as if it were a file in read mode.
import serial
import time
ser = serial.Serial('/dev/ttyUSB0', 9600)
ser.flush()

pipe = open('/dev/input/js0', 'r')

# Create an empty list to store read characters.
msg = []

# Loop forever.
while 1:
    # For each character read from the /dev/input/js0 pipe...
    for char in pipe.read(1):
 
        # append the integer representation of the unicode character read to the msg list.
        msg += [ord(char)]
        
        # If the length of the msg list is 8...
        if len(msg) == 8:

            if msg[6] == 2:
                if (msg[7] == 1 or  msg[7] == 0):
                    dataString = str(msg[7]) + ',' + str(msg[5])
                    ser.write(dataString)
                    ser.write('\n')
                    #receivedmsg = ser.readline()
                    #print receivedmsg
                    print(dataString)
                    ser.flush()
   
            # Reset msg as an empty list.
            msg = []