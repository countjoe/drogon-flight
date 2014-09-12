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
	
	velocityX = 0.0;
	velocityY = 0.0;
	
	varSq = INIT_START_VAR_SQ;
	
	accelVarSq = INIT_ACCEL_VAR_SQ;
	gyroVarSq = INIT_GYRO_VAR_SQ;
	varUpdateScale = INIT_VAR_UPDATE_SCALE;
	velPosUpdateVarSq = INIT_VEL_POS_UPDATE_VAR_SQ;
	velVarSq = INIT_VEL_VAR_SQ;

	sensorVarSq = calc_var( accelVarSq, gyroVarSq );
	
	velocityVarSq = INIT_START_VEL_VAR_SQ;
	
	lastMicros = 0;
}

void DrogonPosition::update( unsigned long micros, const double accelValues[3], const double gyroValues[3] ) {
    if ( lastMicros == 0 || lastMicros >= micros ) {
        // sit this one out
        lastMicros = micros;
        return;
    }

    double elapsedSeconds = ( micros - lastMicros ) / 1000000.0;

	double accelX = (-accelValues[1]*ACCEL_SCALE); // translate to robot coords
	double accelY = (accelValues[0]*ACCEL_SCALE);  // translate to robot coords
	
	// gyroscope is degrees/second, so gyroscope estimated position is:
	double gyroX = x + ( gyroValues[0] * elapsedSeconds );
	double gyroY = y + ( gyroValues[1] * elapsedSeconds );
	
	double lastX = x;
	double lastY = y;
	
	double sensorX = calc_mean( accelX, accelVarSq, gyroX, gyroVarSq );
	double sensorY = calc_mean( accelY, accelVarSq, gyroY, gyroVarSq );
	
	// update current position with velocity
	x = calc_mean( x, varSq, x + ( velocityX * elapsedSeconds ), velPosUpdateVarSq );
    y = calc_mean( y, varSq, y + ( velocityY * elapsedSeconds ), velPosUpdateVarSq );
	
    // don't update position variance, don't want to increase
    //varSqX = calc_var( varSqX, VEL_POS_UPDATE_VAR_SQ );
    //varSqY = calc_var( varSqY, VEL_POS_UPDATE_VAR_SQ );

    // apply sensor position to current position
	x = calc_mean( x, varSq, sensorX, sensorVarSq );
	y = calc_mean( y, varSq, sensorY, sensorVarSq );
	
	//varSq = calc_var( varSq, sensorVarSq );
	
	//varSq *= varUpdateScale;
	
	if ( elapsedSeconds > 0.0 ) {
		velocityX = calc_mean( velocityX, velocityVarSq, ( x - lastX ) / elapsedSeconds, velVarSq );
		velocityY = calc_mean( velocityY, velocityVarSq, ( y - lastY ) / elapsedSeconds, velVarSq );
		
		//velocityVarSq = calc_var( velocityVarSq, velVarSq );
		
		//velocityVarSq *= varUpdateScale;
	}
	
	lastMicros = micros;
}

void DrogonPosition::set_accel_var_sq( double accelVarSq ) {
    this->accelVarSq = accelVarSq;
    sensorVarSq = calc_var( accelVarSq, gyroVarSq );
}
double DrogonPosition::get_accel_var_sq( void ) {
    return accelVarSq;
}

void DrogonPosition::set_gyro_var_sq( double gyroVarSq ) {
    this->gyroVarSq = gyroVarSq;
    sensorVarSq = calc_var( accelVarSq, gyroVarSq );
}

double DrogonPosition::get_gyro_var_sq( void ) {
    return gyroVarSq;
}

void DrogonPosition::set_variance_update_scale( double varUpdateScale ) {
    this->varUpdateScale = varUpdateScale;
}

double DrogonPosition::get_variance_update_scale( void ) {
    return varUpdateScale;
}

void DrogonPosition::set_velocity_position_update_var_sq( double velPosUpdateVarSq ) {
    this->velPosUpdateVarSq = velPosUpdateVarSq;
}

double DrogonPosition::get_velocity_position_update_var_sq( void ) {
    return velPosUpdateVarSq;
}

void DrogonPosition::set_velocity_var_sq( double velVarSq ) {
    this->velVarSq = velVarSq;
}

double DrogonPosition::get_velocity_var_sq( void ) {
    return velVarSq;
}

double DrogonPosition::calc_mean( double mean1, double var1, double mean2, double var2 ) {
	return ( var2 * mean1 + var1 * mean2 ) / ( var1 + var2 );
}

double DrogonPosition::calc_var( double var1, double var2 ) {
	return 1 / ( ( 1 / var1 ) + ( 1 / var2 ) );
}

