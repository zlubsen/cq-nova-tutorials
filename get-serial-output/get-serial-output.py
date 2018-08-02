import serial

if __name__ == "__main__":
    ser = serial.Serial('COM4')

    with open("out.txt", 'w', newline='\r\n') as file:
        while True:
            file.write(ser.readline().decode("utf-8"));
