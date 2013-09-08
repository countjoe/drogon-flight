#!/usr/bin/env python

from ctypes import cdll, c_double, c_int, c_long

SONAME="./libdrogonflight.so"

axis = c_double*3

def update( ax, ay, gx, gy ):
	dc.position_update( c_long(0), axis( ax, ay, 0.0 ), axis( gx, gy, 0.0 ) )
	print "x=%.4f y=%.4f" % ( dc.get_position_x(), dc.get_position_y() )

if __name__ == "__main__":
	dc = cdll.LoadLibrary(SONAME)
	dc.get_position_x.restype = c_double
	dc.get_position_y.restype = c_double
	
	update( 2.0*6.2, -2.0*6.2, 1.0, 1.0 )
	update( 4.0*6.2, -4.0*6.2, 3.0, 3.0 )
	update( 5.0*6.2, -5.0*6.2, 2.0, 2.0 )
	update( 5.0*6.2, -5.0*6.2, 0.2, 0.2 )
	update( 5.0*6.2, -5.0*6.2, 0.1, 0.1 )
	update( 5.0*6.2, -5.0*6.2, -0.1, -0.1 )
	update( 5.0*6.2, -5.0*6.2, 0.2, 0.2 )
	update( 5.0*6.2, -5.0*6.2, -0.01, -0.01 )
	update( 5.0*6.2, -5.0*6.2, 0.2, 0.2 )
	update( 5.0*6.2, -5.0*6.2, 0.05, 0.05 )
	update( 5.0*6.2, -5.0*6.2, 0.1, 0.1 )
	update( 5.0*6.2, -5.0*6.2, -0.1, -0.1 )
	update( 5.0*6.2, -5.0*6.2, 0.1, 0.1 )
	update( 5.0*6.2, -5.0*6.2, -0.1, -0.1 )
	update( 5.0*6.2, -5.0*6.2, 0.1, 0.1 )
