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

#include "DrogonPid.h"
#include "DrogonPidTuner.h"

class DrogonController {
    public:
        DrogonController( DrogonPosition *_position );
        
        void control_update( unsigned long micros, const double target[3] );
        
        DrogonPosition* get_position();
        
        void reset( unsigned long micros );
        void tune();

        double motorAdjusts[4];
		
        double motorOffsetA;
        double motorOffsetB;
        
        DrogonPid pidA;
        DrogonPid pidB;
    private:
        void update_motor_values( unsigned long micros, const double target[3] );
        double array_mult( const double* a, const double* b, int len );
        void map_angles_to_motor_offsets( double targetX, double targetY );
        void rot_matrix_mult( const double* a, const double* b, double* dst );
        
		DrogonPosition* position;

		DrogonPidTuner pidATuner;
		DrogonPidTuner pidBTuner;
        
        bool controlStart;
        
        double motorAOffsetMatrix[3*3];
        double motorBOffsetMatrix[3*3];
        double motorOffsetVector[3];
};

#endif
