#!/usr/bin/env python
'''
Test Motors
'''

import sys
from cmd import Cmd
import rcorelib

class FlightCmd(Cmd):
    '''Flight control command interpreter'''
    def __init__(self, host, *args, **kwargs):
        Cmd.__init__(self, *args, **kwargs)

        self.prompt = '% '

        self.rcore = rcorelib.RCoreClient(host, 'motortest')
        self.rcore.start()

        self.evt_arm = self.rcore.read_event_type('flight_arm')
        self.evt_motor = self.rcore.read_event_type('flight_motor')
        self.evt_pid = self.rcore.read_event_type('flight_pid')
        self.evt_log = self.rcore.read_event_type('flight_log')

    def do_arm(self, arg):
        '''arm command'''
        value = 1
        if len(arg) > 0:
            try:
                value = int(arg)
                self.update_arm(value)
            except ValueError:
                print 'Error: Invalid arm value %s' % (arg)
        else:
            self.update_arm(1)

    def do_disarm(self, arg):
        '''disarm command'''
        self.update_arm(0)

    def do_motor(self, arg):
        '''motor command'''
        if len(arg) > 0:
            try:
                value = float(arg)
                self.update_motor(value)
            except ValueError:
                print 'Error: Invalid arm value %s' % (arg)
        else:
            print 'Error: excepted motor value'

    def do_pid(self, arg):
        '''motor command'''
        parts = arg.split(' ')
        if len(parts) == 3:
            try:
                kp = float(parts[0])
                ki = float(parts[1])
                kd = float(parts[2])
                self.update_pid(kp, ki, kd)
            except ValueError:
                print 'Error: Invalid pid value %s' % (arg)
        else:
            print 'Error: excepted 3 pid values'

    def update_arm(self, value):
        '''Update Arm.'''
        print 'Sending ARM %d' % (value)
        builder = self.evt_arm.buildEvent()
        builder.add_byte(value)
        evt = builder.build()
        self.rcore.send(evt)

    def update_motor(self, value):
        '''Update motor'''
        print 'Sending Motor %f' % (value)
        builder = self.evt_motor.buildEvent()
        builder.add_double(value)
        evt = builder.build()
        self.rcore.send(evt)

    def update_pid(self, kp, ki, kd):
        '''Update Pid'''
        print 'Sending Pid kp=%f ki=%f kd=%f' % (kp, ki, kd)
        builder = self.evt_pid.buildEvent()
        builder.add_double(kp)
        builder.add_double(ki)
        builder.add_double(kd)
        evt = builder.build()
        self.rcore.send(evt)

    def post_loop(self):
        '''post loop'''
        self.update_arm(0)
        print 'Closing RCore'
        self.rcore.close()
        print 'RCore closed'

def main(argv):
    '''
    Main
    '''
    host = 'localhost'
    if len(argv) > 1:
        host = argv[1]

    flightcmd = FlightCmd(host)
    try:
        flightcmd.cmdloop()
    except:
        flightcmd.post_loop()

if __name__ == "__main__":
    main(sys.argv)
