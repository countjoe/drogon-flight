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

#include <math.h>

#ifndef PI
#define PI M_PI
#endif

#define ACCEL_SCALE (1/6.2)

#define INIT_VAR_SQ 4
#define ACCEL_VAR_SQ 1.96
#define GYRO_VAR_SQ 1.96
#define VAR_UPDATE_SCALE 2.0

class DrogonPosition {
	public:
		DrogonPosition(void);
		
		void update( long micros, const double accelValues[3], const double gyroValues[3] );
		
		double x;
		double y;
	private:
		double calc_mean( double mean1, double var1, double mean2, double var2 );
		double calc_var( double var1, double var2 );
		
		double xVarSq;
		double yVarSq;
};

#endif
