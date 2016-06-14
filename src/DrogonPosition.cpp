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
	zero_vector3d(&position);
	zero_vector3d(&velocity);
	zero_vector3d(&accel);
	zero_vector3d(&gyro);
	
	lastUpdated = 0.0;
}

void DrogonPosition::update( double t, vector3d* accelValues, vector3d* gyroValues ) {
    if ( lastUpdated == 0.0 || lastUpdated >= t ) {
        // sit this one out
        lastUpdated = t;
        return;
    }

    double elapsedSeconds = ( t - lastUpdated );

	double accelXUpdate = (accelValues->x * ACCEL_SCALE); // translate to robot coords
	double accelYUpdate = (accelValues->y * ACCEL_SCALE);  // translate to robot coords
	
	// gyroscope is degrees/second, so gyroscope estimated position is:
	double gyroXUpdate = position.x + ( gyroValues->x * elapsedSeconds );
	double gyroYUpdate = position.y + ( gyroValues->y * elapsedSeconds );
	
	vector3d last;
	last.x = position.x;
	last.y = position.y;
	
	accel.x = calc_mean( accel.x, ACCEL_VAR_SQ_A, accelXUpdate, ACCEL_VAR_SQ_B );
	accel.y = calc_mean( accel.y, ACCEL_VAR_SQ_A, accelYUpdate, ACCEL_VAR_SQ_B );

	gyro.x = calc_mean( gyro.x, GYRO_VAR_SQ_A, gyroXUpdate, GYRO_VAR_SQ_B );
	gyro.y = calc_mean( gyro.y, GYRO_VAR_SQ_A, gyroYUpdate, GYRO_VAR_SQ_B );

	double sensorX = calc_mean( accel.x, ACCEL_MERGE_VAR_SQ, gyro.x, GYRO_MERGE_VAR_SQ );
	double sensorY = calc_mean( accel.y, ACCEL_MERGE_VAR_SQ, gyro.y, GYRO_MERGE_VAR_SQ );
	
	// update current position with velocity
	position.x = calc_mean( position.x, POS_VAR_SQ_A, position.x + ( velocity.x * elapsedSeconds ), POS_VAR_SQ_V );
    position.y = calc_mean( position.y, POS_VAR_SQ_A, position.y + ( velocity.y * elapsedSeconds ), POS_VAR_SQ_V );
	
    // don't update position variance, don't want to increase
    //varSqX = calc_var( varSqX, VEL_POS_UPDATE_VAR_SQ );
    //varSqY = calc_var( varSqY, VEL_POS_UPDATE_VAR_SQ );

    // apply sensor position to current position
	position.x = calc_mean( position.x, POS_VAR_SQ_A, sensorX, POS_VAR_SQ_B );
	position.y = calc_mean( position.y, POS_VAR_SQ_A, sensorY, POS_VAR_SQ_B );
	
	position.z = calc_mean( position.z, Z_ROT_VAR_SQ, gyroValues->z, Z_ROT_UPDATE_VAR_SQ );

	//varSq = calc_var( varSq, sensorVarSq );
	
	//varSq *= varUpdateScale;
	
	if ( elapsedSeconds > 0.0 ) {
		velocity.x = calc_mean( velocity.x, VEL_VAR_SQ_A, ( position.x - last.x ) / elapsedSeconds, VEL_VAR_SQ_B );
		velocity.y = calc_mean( velocity.y, VEL_VAR_SQ_A, ( position.y - last.y ) / elapsedSeconds, VEL_VAR_SQ_B );
		
		//velocityVarSq = calc_var( velocityVarSq, velVarSq );
		
		//velocityVarSq *= varUpdateScale;
	}
	
	lastUpdated = t;
}

double DrogonPosition::calc_mean( double mean1, double var1, double mean2, double var2 ) {
	return ( var2 * mean1 + var1 * mean2 ) / ( var1 + var2 );
}

double DrogonPosition::calc_var( double var1, double var2 ) {
	return 1 / ( ( 1 / var1 ) + ( 1 / var2 ) );
}

