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

def calculatePolygon(centerX, centerY, radius, number_sides):
    resultx = []
    resulty = []
    angle = (2*math.pi)/number_sides
    start_angle = (3 * math.pi / 2) + (angle / 2)

    startX = centerX + math.cos(start_angle)*radius
    startY = centerY + math.sin(start_angle)*radius

    resultx.append(startX)
    resulty.append(startY)

    for i in range(0,number_sides):
        tempx = centerX + math.cos(start_angle + i * angle) * radius
        tempy = centerY + math.sin(start_angle + i * angle) * radius
        resultx.append(tempx)
        resulty.append(tempy)

    resultx.append(startX)
    resulty.append(startY)

    return [resultx,resulty]

def processInput(device):
    serialRead = device.readline()
    if serialRead != b'':
        print (int(serialRead))
        return int(serialRead)
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
            temp = chr(faces["feliz"])+chr(cabelo['complexo'])
            Arduino.write(bytearray(temp.encode('latin-1')))

print("ending")

# points = calculatePolygon(0,0,900,3) #generatePoints(x,y)
points2 = generatePoints(x,y)
# plt.plot(points[0],points[1])
plt.plot(points2[0],points2[1])
plt.show()

