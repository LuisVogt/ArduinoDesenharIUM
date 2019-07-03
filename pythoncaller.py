import serial
import matplotlib.pyplot as plt
import math

faces = {
    "feliz":1,
    "triste":2,
    "neutro":3,
    "irritado":4,
    "medo":5,
    "surpreso":6,
    "nojo":7,
    "face":8
}

cabelo = {
    "simples":1,
    "complexo":2
}

def processInput(device):
    serialRead = device.readline()
    if serialRead != b'':
        try:
            print (int(serialRead))
            return int(serialRead)
        except:
            return -15
    return -1

def generatePoints(_x,_y):
    for n in _x:
        print("x")
        print(n)
    for n in _y:
        print("y")
        print(n)
    return [_x,_y]

Arduino = serial.Serial("COM11")
Arduino.timeout = 1

def drawThing(face,cabelo):
    end = False
    is_drawing = False

    is_x = True
    x = []
    y = []

    while not end:
        arduino_input = processInput(Arduino)
        if arduino_input != 58 and arduino_input != 60 and arduino_input != -1:
            if is_x:
                is_x = False
                x.append(int(arduino_input))
            else:
                is_x = True
                y.append(int(arduino_input))
        if arduino_input == 60:
            is_drawing = True
            print("Drawing")
        elif arduino_input == 58:
            if is_drawing:
                end = True
                break
            else:
                temp = chr(face)+chr(cabelo)+chr(0)
                Arduino.write(bytearray(temp.encode('latin-1')))

    points = generatePoints(x,y)
    plt.plot(points[0],points[1])
    plt.show()
