import serial
import time

ser = serial.Serial("/dev/ttyACM0", baudrate="115200");
print ser

while True:
	time.sleep(.0001)
	string = '{ "method": "digitalWrite", "params": [1, 2] }'
	ser.write(string)
