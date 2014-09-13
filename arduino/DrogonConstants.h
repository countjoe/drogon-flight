/*
 * DrogonConstants.h
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

#ifndef __DROGON_CONSTANTS_H__
#define __DROGON_CONSTANTS_H__

#include <math.h>

#ifndef PI
#define PI M_PI
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define INIT_KP 0.001
#define INIT_KI 0.00001
#define INIT_KD 0.0001

#define MAX_ERR_TOTAL 100

#define ARM_LENGTH 300
#define ARM_ANGLE_A (45.0 * PI / 180.0)
#define ARM_ANGLE_B ((90.0+45.0) * PI / 180.0)

#define ACCEL_SCALE (1/6.2)

#define INIT_START_VAR_SQ 4
#define INIT_START_VEL_VAR_SQ 1
#define INIT_ACCEL_VAR_SQ 1.96
#define INIT_GYRO_VAR_SQ 0.64
#define INIT_VAR_UPDATE_SCALE 1.5
#define INIT_VEL_POS_UPDATE_VAR_SQ 2.25
#define INIT_VEL_VAR_SQ 0.25

#define TUNER_INIT 0.25
#define TUNER_TOLERANCE 0.00001
#define TUNER_SCALE_MINOR 1.02
#define TUNER_SCALE_MAJOR 1.1
#define TUNER_SCALE_DOWN 0.9

#endif
