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

DrogonController::DrogonController( void ) {
    thetas[0] = THETAS_KP;
    thetas[1] = THETAS_KI;
    thetas[2] = THETAS_KD;
    
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
    
    zero_motor_values(0);
    
    controlStart = false;
}

void DrogonController::zero_motor_values( long micros ) {
    for ( int i = 0; i < NUM_FEATURES; i++ ) {
        featuresA[i] = 0.0;
        featuresB[i] = 0.0;
    }
    
    motorOffsetA = 0.0;
    motorOffsetB = 0.0;

    lastErrUpdate = micros;

    errTotalA = 0.0;
    errTotalB = 0.0;

    errLastA = 0.0;
    errLastB = 0.0;

    errA = 0.0;
    errB = 0.0;

    motorAdjusts[0] = 0.0;
    motorAdjusts[1] = 0.0;
    motorAdjusts[2] = 0.0;
    motorAdjusts[3] = 0.0;
}

void DrogonController::control_update( long micros ) {
  if ( !controlStart ) {
    zero_motor_values( micros );
    controlStart = true;
  } else {
    update_motor_values( micros );
  }
}

void DrogonController::update_motor_values( long micros ) {
  map_angles_to_motor_offsets();

  calc_errs( micros );
  
  motorAdjusts[0] = -errA;
  motorAdjusts[2] =  errA;
  
  motorAdjusts[1] =  errB;
  motorAdjusts[3] = -errB;
}

/*
 * Calculate errors using PID algorithm from motor offsets (distance to center)
 */
void DrogonController::calc_errs( long micros ) {
  double rawErrA = motorOffsetA;
  double rawErrB = motorOffsetB;
  
  // calculate elapsed time since last error update in seconds
  double elapsed = ( micros - lastErrUpdate ) / 1000000.0;
  
  errTotalA += ( ( errLastA + ( ( rawErrA - errLastA ) / 2.0 ) ) * elapsed );
  errTotalB += ( ( errLastB + ( ( rawErrB - errLastB ) / 2.0 ) ) * elapsed );
  
  // bound err total to min/max to prevent overrunning
  errTotalA = max( MIN_ERR_TOTAL, min( MAX_ERR_TOTAL, errTotalA ) );
  errTotalB = max( MIN_ERR_TOTAL, min( MAX_ERR_TOTAL, errTotalB ) );
  
  // err diff is change in degrees in seconds from last update
  double errDiffA = ( rawErrA - errLastA ) / elapsed;
  double errDiffB = ( rawErrB - errLastB ) / elapsed;
  
  // setup features (PID values) for motor A
  featuresA[0] = rawErrA;     // P component, raw error offset
  featuresA[1] = errTotalA;   // I component, total error
  featuresA[2] = errDiffA;    // D component, rate of change of error
  
  // setup features (PID values) for motor B
  featuresB[0] = rawErrB;     // P component, raw error offset
  featuresB[1] = errTotalB;   // I component, total error
  featuresB[2] = errDiffB;    // D component, rate of change of error
  
  // compute PID function by multiplying feature vector with 
  // thetas vecor (PID constants)
  errA = array_mult( featuresA, thetas, NUM_FEATURES );
  errB = array_mult( featuresB, thetas, NUM_FEATURES );
  
  // store last error values
  errLastA = rawErrA;
  errLastB = rawErrB;
  
  // store time of this update
  lastErrUpdate = micros;
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
void DrogonController::map_angles_to_motor_offsets( void ) {
  double rot1[3];
  double rot2[3];
  double rot3[3];
  
  double zA;
  double zB;
  
  // convert angle in degrees to radians for trig functions
  double angleXRadians = position.x * PI / 180.0;
  double angleYRadians = position.y * PI / 180.0;
  
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


void DrogonController::update_thetas( double kp, double ki, double kd ) {
    thetas[0] = kp;
    thetas[1] = ki;
    thetas[2] = kd;
}

