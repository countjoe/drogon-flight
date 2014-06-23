#!/usr/bin/env python

from ctypes import cdll, c_double, c_int, c_long

SONAME="./libdrogonflight.so"

axis = c_double*3
t = 0

def update( ax, ay, gx, gy ):
	global t
	dc.position_update( c_long(t), axis( ax, ay, 0.0 ), axis( gx, gy, 0.0 ) )
	print "x=%.4f y=%.4f" % ( dc.get_position_x(), dc.get_position_y() )
	t += 5000

if __name__ == "__main__":
	dc = cdll.LoadLibrary(SONAME)
	dc.get_position_x.restype = c_double
	dc.get_position_y.restype = c_double
	
	update( 2.0*6.2, -2.0*6.2, 1.0, 1.0 )
	update( 4.0*6.2, -4.0*6.2, 1.5, 1.5 )
	update( 5.0*6.2, -5.0*6.2, 0.3, 0.3 )
	update( 5.0*6.2, -5.0*6.2, 0.2, 0.2 )
	update( 5.0*6.2, -6.0*6.2, 0.1, 0.1 )
	update( 5.0*6.2, -5.0*6.2, -0.1, -0.1 )
	update( 5.0*6.2, -12.0*6.2, 0.2, 0.2 )
	update( 5.0*6.2, -22.0*6.2, -0.01, -0.01 )
	update( 5.0*6.2, -5.0*6.2, 0.2, 0.2 )
	update( 5.0*6.2, -5.0*6.2, 0.05, 0.05 )
	update( 5.0*6.2, -5.0*6.2, 0.1, 0.1 )
	update( 5.0*6.2, 2.0*6.2, -0.1, -0.1 )
	update( 5.0*6.2, -5.0*6.2, 0.1, 0.1 )
	update( 5.0*6.2, -5.0*6.2, -0.1, -0.1 )
	update( 5.0*6.2, -5.0*6.2, 0.1, 0.1 )
	update( 5.0*6.2, -5.0*6.2, 0.1, 0.1 )
	update( 5.0*6.2, -5.0*6.2, 0.1, 0.1 )
	update( 5.0*6.2, -5.0*6.2, 0.0, 0.0 )
	update( 5.0*6.2, -5.0*6.2, 0.1, 0.0 )
	update( 5.0*6.2, -5.0*6.2, 0.1, 0.0 )
	update( 5.0*6.2, -5.0*6.2, 0.1, 0.1 )
	update( 5.0*6.2, -5.0*6.2, 0.1, 0.1 )
