/*
 * DrogonController.h
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

#ifndef __DROGON_CONTROLLER_H__
#define __DROGON_CONTROLLER_H__

#include "DrogonPosition.h"

#include <math.h>

#ifndef PI
#define PI M_PI
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define NUM_MOTORS 4

#define NUM_FEATURES 3

#define THETAS_KP 0.05
#define THETAS_KI 0.002
#define THETAS_KD 0.001

#define MAX_ERR_TOTAL 5
#define MIN_ERR_TOTAL -MAX_ERR_TOTAL

#define ARM_LENGTH 300
#define ARM_ANGLE_A (45.0 * PI / 180.0)
#define ARM_ANGLE_B ((90.0+45.0) * PI / 180.0)

class DrogonController {
    public:
        DrogonController( DrogonPosition *_position );
        
        void control_update( long micros );
        
        void update_thetas( double kp, double ki, double kd );
        
        DrogonPosition* get_position();
        
        double motorAdjusts[NUM_MOTORS];
		
        double motorOffsetA;
        double motorOffsetB;
        
		double featuresA[NUM_FEATURES];
		double featuresB[NUM_FEATURES];
        
        double errA;
        double errB;
    private:
        void zero_motor_values( long micros );
        void update_motor_values( long micros );
        void calc_errs( long micros );
        double array_mult( const double* a, const double* b, int len );
        void map_angles_to_motor_offsets( void );
        void rot_matrix_mult( const double* a, const double* b, double* dst );
        
		DrogonPosition* position;

        double thetas[NUM_FEATURES];
        
        bool controlStart;
        long lastErrUpdate;
        double errTotalA;
        double errTotalB;

        double errLastA;
        double errLastB;
        
        double motorAOffsetMatrix[3*3];
        double motorBOffsetMatrix[3*3];
        double motorOffsetVector[3];
};

#endif
