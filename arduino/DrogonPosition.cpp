
#include "DrogonPosition.h"

DrogonPosition::DrogonPosition(void) {
	x = 0.0;	
	y = 0.0;
	
	xVarSq = INIT_VAR_SQ;
	yVarSq = INIT_VAR_SQ;
}

void DrogonPosition::update( long micros, const double accelValues[3], const double gyroValues[3] ) {
	double accelX = (-accelValues[1]*ACCEL_SCALE);
	double accelY = (accelValues[0]*ACCEL_SCALE);
	
	double gyroX = gyroValues[0];
	double gyroY = gyroValues[1];
	
	x = calc_mean( x, xVarSq, accelX, ACCEL_VAR_SQ );
	y = calc_mean( y, yVarSq, accelY, ACCEL_VAR_SQ );
	
	xVarSq = calc_var( xVarSq, ACCEL_VAR_SQ );
	yVarSq = calc_var( yVarSq, ACCEL_VAR_SQ );

	x = calc_mean( x, xVarSq, gyroX, GYRO_VAR_SQ );
	y = calc_mean( y, yVarSq, gyroY, GYRO_VAR_SQ );
	
	xVarSq = calc_var( xVarSq, GYRO_VAR_SQ );
	yVarSq = calc_var( yVarSq, GYRO_VAR_SQ );
	
	xVarSq *= VAR_UPDATE_SCALE;
	yVarSq *= VAR_UPDATE_SCALE;
}

double DrogonPosition::calc_mean( double mean1, double var1, double mean2, double var2 ) {
	return ( var2 * mean1 + var1 * mean2 ) / ( var1 + var2 );
}

double DrogonPosition::calc_var( double var1, double var2 ) {
	return ( 1 / ( 1 / var1 ) + ( 1 / var2 ) );
}

