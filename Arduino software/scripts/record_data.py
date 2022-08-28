import serial
import time

# ser = serial.Serial('/dev/cu.usbserial-14430', 9600, timeout=1)
ser = serial.Serial('/dev/cu.usbmodem144301', 250000, timeout=9)

file=open('./test.txt','w')
# for i in range(19):
#     ser.write(bytes('9', 'utf-8'))
#     time.sleep(0.05)
 

while(True):
    line = ser.readline() 
    string = line.decode()  # convert the byte string to a unicode string
    t1=time.time()
    file.write(str(t1)+"\t"+string+"\n")
    # print(string)
    