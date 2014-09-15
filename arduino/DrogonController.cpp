/*
 * DrogonController.cpp
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

#include "DrogonController.h"

#include "DrogonConstants.h"

DrogonController::DrogonController( DrogonPosition *_position ) :
            pidA(INIT_KP, INIT_KI, INIT_KD),
            pidB(INIT_KP, INIT_KI, INIT_KD),
            pidATuner(&pidA),
            pidBTuner(&pidB) {

    pidA.set_max_sum( MAX_ERR_TOTAL );
    pidB.set_max_sum( MAX_ERR_TOTAL );

    motorAOffsetMatrix[0] = cos(ARM_ANGLE_A);
    motorAOffsetMatrix[1] = -sin(ARM_ANGLE_A);
    motorAOffsetMatrix[2] = 0;
    motorAOffsetMatrix[3] = sin(ARM_ANGLE_A);
    motorAOffsetMatrix[4] = cos(ARM_ANGLE_A);
    motorAOffsetMatrix[5] = 0;
    motorAOffsetMatrix[6] = 0;
    motorAOffsetMatrix[7] = 0;
    motorAOffsetMatrix[8] = 1;
    
    motorBOffsetMatrix[0] = cos(ARM_ANGLE_B);
    motorBOffsetMatrix[1] = -sin(ARM_ANGLE_B);
    motorBOffsetMatrix[2] = 0;
    motorBOffsetMatrix[3] = sin(ARM_ANGLE_B);
    motorBOffsetMatrix[4] = cos(ARM_ANGLE_B);
    motorBOffsetMatrix[5] = 0;
    motorBOffsetMatrix[6] = 0;
    motorBOffsetMatrix[7] = 0;
    motorBOffsetMatrix[8] = 1;
    
    motorOffsetVector[0] = 0;
    motorOffsetVector[1] = ARM_LENGTH;
    motorOffsetVector[2] = 0;
    
    reset(0);
    
    position = _position;
}

void DrogonController::reset( unsigned long micros ) {
    pidA.reset( micros );
    pidB.reset( micros );
    
    pidATuner.reset();
    pidBTuner.reset();

    motorOffsetA = 0.0;
    motorOffsetB = 0.0;

    motorAdjusts[0] = 0.0;
    motorAdjusts[1] = 0.0;
    motorAdjusts[2] = 0.0;
    motorAdjusts[3] = 0.0;

    controlStart = false;
}

void DrogonController::tune( void ) {
    pidATuner.tune();
    pidBTuner.tune();
}

void DrogonController::control_update( unsigned long micros, const double target[3] ) {
    if ( !controlStart ) {
        reset( micros );
        controlStart = true;
    } else {
        update_motor_values( micros, target );
    }
}

void DrogonController::update_motor_values( unsigned long micros, const double target[3] ) {
    map_angles_to_motor_offsets( target[0], target[1] );

    double errA = pidA.update( micros, motorOffsetA );
    double errB = pidB.update( micros, motorOffsetB );

    pidATuner.update( motorOffsetA );
    pidATuner.update( motorOffsetB );

    motorAdjusts[0] = -errA;
    motorAdjusts[2] =  errA;

    motorAdjusts[1] =  errB;
    motorAdjusts[3] = -errB;
}

DrogonPosition* DrogonController::get_position() {
	return position;
}


/*
 * multiply arrays. like multiplying matrices if b was transposed.
 */
double DrogonController::array_mult( const double* a, const double* b, int len ) {
    double value = 0.0;
    for ( int i = 0; i < len; i++ ) {
        value += ( a[i] * b[i] );
    }
    return value;
}

/*
 * Map angle to motor offsets.
 */
void DrogonController::map_angles_to_motor_offsets( double targetX, double targetY ) {
    double rot1[3];
    double rot2[3];
    double rot3[3];

    double zA;
    double zB;

    // convert angle in degrees to radians for trig functions
    double angleXRadians = (position->x - targetX) * PI / 180.0;
    double angleYRadians = (position->y - targetY) * PI / 180.0;

    // compute sines/cosines for rotation matrices
    double sinAngleX = sin(angleXRadians);
    double sinAngleY = sin(angleYRadians);
    double cosAngleX = cos(angleXRadians);
    double cosAngleY = cos(angleYRadians);

    // X rotation matrix
    double motorXOffsetMatrix[] = {
        1, 0,          0,
        0, cosAngleX, -sinAngleX,
        0, sinAngleX,  cosAngleX
    };

    // Y rotation matrix
    double motorYOffsetMatrix[] = {
         cosAngleY, 0, sinAngleY,
         0,         1, 0,
        -sinAngleY, 0, cosAngleY
    };

    // rotate motor A around Z axis (each motor arm offset 45 degrees)
    rot_matrix_mult( motorAOffsetMatrix, motorOffsetVector, rot1 );

    // rotate around X axis
    rot_matrix_mult( motorXOffsetMatrix, rot1, rot2 );

    // rotate around Y axis
    rot_matrix_mult( motorYOffsetMatrix, rot2, rot3 );

    // extract Z (height) of motor A
    zA = rot3[2];

    // rotate motor B around Z axis (each motor arm offset 45 degrees)
    rot_matrix_mult( motorBOffsetMatrix, motorOffsetVector, rot1 );

    // rotate around X axis
    rot_matrix_mult( motorXOffsetMatrix, rot1, rot2 );

    // rotate around Y axis
    rot_matrix_mult( motorYOffsetMatrix, rot2, rot3 );

    // extract Z (height) of motor B
    zB = rot3[2];

    // Convert Z (height) to angle then to arc length to accurately
    // reprsent distance of motor to center.
    //  Variables:
    //    OPP: z (height) of arm (known from rotations)
    //    ADJ: length of arm (known from constant)
    //    ANG: angle of arm (unknown, computed)
    //    R: length of arm (known constant)
    //    ARCLEN: distance of motor to center (computed)
    //  Formulas:
    //    ANG = arcsin(OPP/ADJ))   <-- from sin(ANG) = OPP/ADJ
    //    ARCLEN = ANG * R
    motorOffsetA = asin(zA/ARM_LENGTH) * ARM_LENGTH;
    motorOffsetB = asin(zB/ARM_LENGTH) * ARM_LENGTH;
}

/*
 * Multiple rotation matrix by XYZ vector. Write to dst vector rotated XYZ.
 */
void DrogonController::rot_matrix_mult( const double* a, const double* b, double* dst ) {
    for ( int i = 0; i < 3; i++ ) {
        dst[i] = 0.0;
        for ( int j = 0; j < 3; j++ ) {
            dst[i] += ( a[i*3+j] * b[j] );
        }
    }
}


