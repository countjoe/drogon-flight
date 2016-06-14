/*
 * DrogonPosition.h
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

#ifndef __DROGON_POSITION_H__
#define __DROGON_POSITION_H__

#include "DrogonCommon.h"

class DrogonPosition {
	public:
		DrogonPosition(void);
		
		void update( double t, vector3d* accelValues, vector3d* gyroValues );
		
		void set_accel_var_sq( double accelVarSq );
		double get_accel_var_sq( void );

        void set_gyro_var_sq( double gyroVarSq );
        double get_gyro_var_sq( void );

        void set_variance_update_scale( double varUpdateScale );
        double get_variance_update_scale( void );

        void set_velocity_position_update_var_sq( double velPosUpdateVarSq );
        double get_velocity_position_update_var_sq( void );

        void set_velocity_var_sq( double velVarSq );
        double get_velocity_var_sq( void );

		vector3d position;
		vector3d velocity;
	private:
		double calc_mean( double mean1, double var1, double mean2, double var2 );
		double calc_var( double var1, double var2 );
		
        vector3d accel;
        vector3d gyro;

		double lastUpdated;
};

#endif
