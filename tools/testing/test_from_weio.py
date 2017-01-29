import serial
import time
import json

GPIO_IN = 0
GPIO_IN_PD = 1
GPIO_IN_PU = 2
GPIO_OUT = 3

connected = False
while not connected:
    try:
        ser = serial.Serial("/dev/ttyACM0", baudrate="115200");
        print ser
        connected = True
    except:
        time.sleep(0.1)

    while True:
        string = '{ "method": "pinMode", "params": [%d, %d] }'%(46, GPIO_OUT)

        ser.write(string)
        size = 0
        while not size:
            size = ser.inWaiting()

        a = json.loads(ser.read(size))
        print a["Status"]

        string = '{ "method": "pinMode", "params": [%d, %d] }'%(9, GPIO_IN_PU)

        ser.write(string)
        size = 0
        while not size:
            size = ser.inWaiting()
        a = json.loads(ser.read(size))
        print a["Status"]

        string = '{ "method": "digitalRead", "params": [%d] }'%(9)

        ser.write(string)
        size = 0
        while not size:
            size = ser.inWaiting()
        a = json.loads(ser.read(size))
        print a["value"]

        string = '{ "method": "digitalWrite", "params": [%d, %d] }'%(46, 1)
        ser.write(string)
        size = 0
            while not size:
        size = ser.inWaiting()
        a = json.loads(ser.read(size))
        print a["Status"]

        #time.sleep(0.1)

        string = '{ "method": "digitalWrite", "params": [%d, %d] }'%(46, 0)
        ser.write(string)
        size = 0
        while not size:
            size = ser.inWaiting()
        a = json.loads(ser.read(size))
        print a["Status"]

        #time.sleep(0.1)

