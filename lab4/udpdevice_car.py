from twisted.internet import reactor
from udpwkpf import WuClass, Device
import sys
from udpwkpf_io_interface import *
import time

IR1_pin = 1
IR2_pin = 2

class Car(WuClass):
    def __init__(self):
        WuClass.__init__(self)
        self.loadClass('Car_Controller')
        self.permission = False
        self.IR1_aio = pin_mode(IR1_pin, PIN_TYPE_ANALOG)
        self.IR2_aio = pin_mode(IR2_pin, PIN_TYPE_ANALOG)
        self.motor1 = mraa.Pwm(3)
        self.motor2 = mraa.Pwm(5)
        self.motor1.period_us(3000)
        self.motor2.period_us(3000)
        self.motor1.pulsewidth_us(1400)
        self.motor2.pulsewidth_us(1600)
        self.motor1.enable(False)
        self.motor2.enable(False)
        

    def checkCross(self):
        IR1 = analog_read(self.IR1_aio)
        IR2 = analog_read(self.IR2_aio)
        print "IR1: ", IR1, ", IR2: ", IR2
        return (IR1>500)and(IR2>450)
    
    def GO(self):
        print "GO"
        # check turn
        IR1 = analog_read(self.IR1_aio)
        IR2 = analog_read(self.IR2_aio)
        #print "IR1: ", IR1, ", IR2: ", IR2
        if (IR1>500):
            # Right in Black, turn right
            print "R"
            self.motor1.enable(False)
            self.motor2.enable(True)
        elif (IR2>450):
            # Left in Black, turn left
            print "L"
            self.motor1.enable(True)
            self.motor2.enable(False)
        else :
            # Forward
            print "F"
            self.motor1.enable(True)
            self.motor2.enable(True)
        
    def STOP(self):
        self.motor1.enable(False)
        self.motor2.enable(False)
        print "STOP"
    
    def setup(self, obj):
        print "refresh setup"
        reactor.callLater(1, self.refresh, obj)
        
    def refresh(self, obj):
        print "refresh"
        self.update( obj, 3, 0) #3 for internal call
        reactor.callLater(1, self.refresh, obj)
    
    def update(self,obj,pID=None,val=None):
        print "update"
        print "pID: " + str(pID)+" val: " + str(val)
        try:
            if pID == 0:
                self.permission = val
                print "Get permission"
            onCross = self.checkCross()
            obj.setProperty(1, onCross)
            if self.permission or not onCross:
                self.GO()
            else:
                self.STOP()
            
        except IOError:
            print "Error"

if __name__ == "__main__":
    class MyDevice(Device):
        def __init__(self,addr,localaddr):
            Device.__init__(self,addr,localaddr)
            
        def init(self):
            m = Car()
            self.addClass(m,0)
            self.objcar = self.addObject(m.ID)
            m.setup( self.objcar)
            
    if len(sys.argv) <= 2:
        print 'python udpwkpf.py <ip> <port>'
        print '      <ip>: IP of the interface'
        print '      <port>: The unique port number in the interface'
        print ' ex. python udpwkpf.py 127.0.0.1 3000'
        sys.exit(-1)

    d = MyDevice(sys.argv[1],sys.argv[2])
    reactor.run()
    device_cleanup()
