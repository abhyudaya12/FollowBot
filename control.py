import pyfirmata
import time
from socket import *

HOST = '192.168.137.78'
PORT = 21567
BUFSIZE = 1024        
sock = socket(AF_INET,SOCK_DGRAM)
sock.bind((HOST,PORT))

t=0.02
tim =0.1
board = pyfirmata.Arduino('/dev/ttyACM0')
pin2 = board.get_pin('d:2:o')
pin3 = board.get_pin('d:3:o')
pin4 = board.get_pin('d:4:o')
pin5 = board.get_pin('d:5:o')
pin6 = board.get_pin('d:6:o')    

def stop():
    pin2.write(0)
    pin3.write(0)
    pin4.write(0)
    pin5.write(0)
    
def left():
    pin6.write(1)
    pin2.write(0)
    pin3.write(1)
    pin4.write(1)
    pin5.write(0)
    time.sleep(t)
    stop();

def right():
    pin6.write(1)
    pin3.write(0)
    pin2.write(1)
    pin5.write(1)
    pin4.write(0)
    time.sleep(t)
    stop();
def forward(tm):
    pin3.write(1)
    pin2.write(0)
    pin5.write(1)
    pin4.write(0)
    time.sleep(tm)
    stop()

def backward(tm):
    pin3.write(0)
    pin2.write(1)
    pin5.write(0)
    pin4.write(1)
    time.sleep(tm)
    stop()
    
data, addr = sock.recvfrom(BUFSIZE)
curval=int(data.split(',')[0])
jj=0
jp=0
while True:
    data, addr = sock.recvfrom(BUFSIZE)
    #print data
    if data=="":
        print "no data"
    else:
        
        values=data.split(',')
        if int(values[0]) < int(curval)-2:
            #print values[0]
            left()
            curval=values[0]
            
        if int(values[0]) > int(curval)+2:
            right()
            curval=values[0]
        print float(values[1])
        if float(values[1]) <1:
            jj=0
            stop()
        else:
            #if(jj <= 4):
            forward(0.001)
            #jj+=4
        
        
            
        '''acc=round(float(values[1]),1);
        s=0.5*acc*tim*tim*100
        print s
        if s*0.2/11 >= 0.0 and s*0.2/11<=0.05:
            stop()
        else:
            forward(s*0.2/11)
        '''
            
            

sock.close();

