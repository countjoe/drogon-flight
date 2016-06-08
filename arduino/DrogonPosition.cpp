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

#include "DrogonConstants.h"

DrogonPosition::DrogonPosition(void) {
	x = 0.0;	
	y = 0.0;
	
	zRot = 0.0;

	velocityX = 0.0;
	velocityY = 0.0;
	
	lastUpdated = 0;
	accelX = 0.0;
	accelY = 0.0;
	gyroX = 0.0;
	gyroY = 0.0;
}

void DrogonPosition::update( long long nanos, const double accelValues[3], const double gyroValues[3] ) {
    if ( lastUpdated == 0 || lastUpdated >= nanos ) {
        // sit this one out
        lastUpdated = nanos;
        return;
    }

    double elapsedSeconds = ( nanos - lastUpdated ) / 1000000000.0;

	double accelXUpdate = (accelValues[0]*ACCEL_SCALE); // translate to robot coords
	double accelYUpdate = (accelValues[1]*ACCEL_SCALE);  // translate to robot coords
	
	// gyroscope is degrees/second, so gyroscope estimated position is:
	double gyroXUpdate = x + ( gyroValues[0] * elapsedSeconds );
	double gyroYUpdate = y + ( gyroValues[1] * elapsedSeconds );
	
	double lastX = x;
	double lastY = y;
	
	accelX = calc_mean( accelX, ACCEL_VAR_SQ_A, accelXUpdate, ACCEL_VAR_SQ_B );
	accelY = calc_mean( accelY, ACCEL_VAR_SQ_A, accelYUpdate, ACCEL_VAR_SQ_B );

	gyroX = calc_mean( gyroX, GYRO_VAR_SQ_A, gyroXUpdate, GYRO_VAR_SQ_B );
	gyroY = calc_mean( gyroY, GYRO_VAR_SQ_A, gyroYUpdate, GYRO_VAR_SQ_B );

	double sensorX = calc_mean( accelX, ACCEL_MERGE_VAR_SQ, gyroX, GYRO_MERGE_VAR_SQ );
	double sensorY = calc_mean( accelY, ACCEL_MERGE_VAR_SQ, gyroY, GYRO_MERGE_VAR_SQ );
	
	// update current position with velocity
	x = calc_mean( x, POS_VAR_SQ_A, x + ( velocityX * elapsedSeconds ), POS_VAR_SQ_V );
    y = calc_mean( y, POS_VAR_SQ_A, y + ( velocityY * elapsedSeconds ), POS_VAR_SQ_V );
	
    // don't update position variance, don't want to increase
    //varSqX = calc_var( varSqX, VEL_POS_UPDATE_VAR_SQ );
    //varSqY = calc_var( varSqY, VEL_POS_UPDATE_VAR_SQ );

    // apply sensor position to current position
	x = calc_mean( x, POS_VAR_SQ_A, sensorX, POS_VAR_SQ_B );
	y = calc_mean( y, POS_VAR_SQ_A, sensorY, POS_VAR_SQ_B );
	
	zRot = calc_mean( zRot, Z_ROT_VAR_SQ, gyroValues[2], Z_ROT_UPDATE_VAR_SQ );

	//varSq = calc_var( varSq, sensorVarSq );
	
	//varSq *= varUpdateScale;
	
	if ( elapsedSeconds > 0.0 ) {
		velocityX = calc_mean( velocityX, VEL_VAR_SQ_A, ( x - lastX ) / elapsedSeconds, VEL_VAR_SQ_B );
		velocityY = calc_mean( velocityY, VEL_VAR_SQ_A, ( y - lastY ) / elapsedSeconds, VEL_VAR_SQ_B );
		
		//velocityVarSq = calc_var( velocityVarSq, velVarSq );
		
		//velocityVarSq *= varUpdateScale;
	}
	
	lastUpdated = nanos;
}

double DrogonPosition::calc_mean( double mean1, double var1, double mean2, double var2 ) {
	return ( var2 * mean1 + var1 * mean2 ) / ( var1 + var2 );
}

double DrogonPosition::calc_var( double var1, double var2 ) {
	return 1 / ( ( 1 / var1 ) + ( 1 / var2 ) );
}

