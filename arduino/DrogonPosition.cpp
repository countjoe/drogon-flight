/*
 * DrogonPosition.cpp
 * 
 * This file is part of Drogon.
 *
 * Drogon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Drogon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Drogon.  If not, see <http://www.gnu.org/licenses/>. 
 *
 * Author: Joseph Monti <joe.monti@gmail.com>
 * Copyright (c) 2013 Joseph Monti All Rights Reserved, http://joemonti.org/
 */

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

