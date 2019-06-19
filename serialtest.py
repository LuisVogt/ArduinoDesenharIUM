import serial

faces = {
    "feliz":1,
    "triste":2,
    "neutro":3,
    "irritado":4,
    "medo":5,
    "surpreso":6,
    "nojo":7
}

cabelo = {
    "normal":1,
}

def processInput(device):
    serialRead = device.read()
    if serialRead != b'':
        return serialRead
    return -1

Arduino = serial.Serial("COM8")
Arduino.timeout = 1

end = False

while not end:
    arduino_input = processInput(Arduino)
    print(arduino_input)
    if arduino_input == b'60':
        print("Drawing")
    elif arduino_input == b':':
        temp = chr(faces["feliz"]) + chr(cabelo["normal"])
        Arduino.write(bytearray(temp.encode('latin-1')))
