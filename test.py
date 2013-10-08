#!/usr/bin/env python

from ctypes import cdll, c_double, c_int, c_long

SONAME="./libdrogonflight.so"


def debug_dc( millis, dc, KP, KI, KD ):
	#print "t=%i\tx=%.4f\ty=%.4f\terrA=%.4f\terrB=%.4f\tadj=[%.4f %.4f %.4f %.4f]" % ( millis, dc.get_position_x(), dc.get_position_y(), dc.get_err_a(), dc.get_err_b(), dc.get_motor_adjust(c_int(0)), dc.get_motor_adjust(c_int(1)), dc.get_motor_adjust(c_int(2)), dc.get_motor_adjust(c_int(3)) )
	print "t=%06i\tx=%.4f\ty=%.4f\toffA=%.4f\terrA=%.4f\tfeatA=[%.4f %.4f %.4f]\terrA=[%.4f %.4f %.4f]" % ( millis, dc.get_position_x(), dc.get_position_y(), dc.get_motor_offset_a(), dc.get_err_a(), dc.get_pid_a_error_p(), dc.get_pid_a_error_i(), dc.get_pid_a_error_d(), dc.get_pid_a_error_p() * KP, dc.get_pid_a_error_i() * KI, dc.get_pid_a_error_d() * KD )

if __name__ == "__main__":
	dc = cdll.LoadLibrary(SONAME)
	dc.get_position_x.restype = c_double
	dc.get_position_y.restype = c_double
	dc.get_motor_offset_a.restype = c_double
	dc.get_motor_offset_b.restype = c_double
	dc.get_err_a.restype = c_double
	dc.get_err_b.restype = c_double
	dc.get_motor_adjust.restype = c_double
	dc.get_pid_a_error_p.restype = c_double
	dc.get_pid_a_error_i.restype = c_double
	dc.get_pid_a_error_d.restype = c_double
	dc.get_pid_b_error_p.restype = c_double
	dc.get_pid_b_error_i.restype = c_double
	dc.get_pid_b_error_d.restype = c_double
	
	KP = 0.25
	KI = 0.2
	KD = 0.05
	
	dc.update_thetas( c_double(KP), c_double(KI), c_double(KD) )
	
	t = 0
	STEP = 5000	
	STOP = 1000000
	x = 0.0
	y = 0.0
	X_STEP = 0.05
	Y_STEP = 0
	while t <= STOP:
		dc.set_position( c_double(x), c_double(y) )
		dc.control_update( c_long(t) )
		debug_dc(t, dc, KP, KI, KD)
		t += STEP
		x += X_STEP
		y += Y_STEP
	
	for i in range(800):
		dc.set_position( c_double(x), c_double(y) )
		dc.control_update( c_long(t) )
		debug_dc(t, dc, KP, KI, KD)
		t += STEP
		
	
