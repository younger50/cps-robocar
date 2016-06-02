from twisted.internet import reactor
from udpwkpf import WuClass, Device
import sys
#from udpwkpf_io_interface import *

import time

class Traffic(WuClass):
    def __init__(self):
        WuClass.__init__(self)
        self.loadClass('Traffic_Controller')
        self.startTime = time.time()
        self.lights = ["red", "green"]
        self.mainLight = self.lights[0]
        self.secondaryLight = self.lights[0]
        self.timeCount = 0
        self.mainTime = [60, 90]
        self.mainChanges = [0, 1]
        self.secondaryTime = [65, 85]
        self.secondaryChanges = [1, 0]
        self.permissionChanges = [True, False]
        self.period = 90
        self.given = False

    def setup(self, obj):
        print "refresh setup"
        reactor.callLater(1, self.refresh, obj)
        
    def refresh(self, obj):
        print "refresh"
        print "Time: " + str(self.timeCount)
        changed = False
        self.timeCount += 1
        if self.timeCount in self.mainTime:
            self.mainLight = self.lights[self.mainChanges[self.mainTime.index(self.timeCount)]]
            changed = True
        if self.timeCount in self.secondaryTime:
            index = self.secondaryTime.index(self.timeCount)
            self.secondaryLight = self.lights[self.secondaryChanges[index]]
            obj.setProperty(0, self.permissionChanges[index])
            self.given = self.permissionChanges[index]
            changed = True
        if changed:
            print "main: " + self.mainLight + ", sub: " + self.secondaryLight
        if self.timeCount == self.period:
            self.timeCount = 0
        reactor.callLater(1, self.refresh, obj)

    def update(self,obj,pID=None,val=None):
        try:
            if pID == 1 and val == True:
                print "Requested"
                if not self.given:
                    self.timeCount = 59
                
        except IOError:
            print "Error"

if __name__ == "__main__":
    class MyDevice(Device):
        def __init__(self,addr,localaddr):
            Device.__init__(self,addr,localaddr)
            
        def init(self):
            m = Traffic()
            self.addClass(m,0)
            self.objtraffic = self.addObject(m.ID)
            m.setup( self.objtraffic)

    if len(sys.argv) <= 2:
        print 'python udpwkpf.py <ip> <port>'
        print '      <ip>: IP of the interface'
        print '      <port>: The unique port number in the interface'
        print ' ex. python udpwkpf.py 127.0.0.1 3000'
        sys.exit(-1)

    d = MyDevice(sys.argv[1],sys.argv[2])
    reactor.run()
    device_cleanup()
