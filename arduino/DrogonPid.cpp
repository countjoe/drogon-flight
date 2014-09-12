/*
 * DrogonPid.cpp
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

#include "DrogonPid.h"

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

DrogonPid::DrogonPid( double kp, double ki, double kd ) {
    this->k[0] = kp;
    this->k[1] = ki;
    this->k[2] = kd;

    maxSum = 0.0;

    reset( 0 );
}

double DrogonPid::update( unsigned long micros, double value ) {
    if ( lastUpdated == 0 || micros < lastUpdated ) {
        // don't update if haven't yet updated or overflowed
        lastUpdated = micros;
        return error;
    }

    e[0] = value;

    // calculate elapsed time since last error update in seconds
    double elapsed = ( micros - lastUpdated ) / 1000000.0;

    e[1] += ( ( errLast + ( ( e[0] - errLast ) / 2.0 ) ) * elapsed );

    // bound err total to min/max to prevent overrunning
    if ( maxSum != 0.0 ) {
        e[1] = max( -maxSum, min( maxSum, e[1] ) );
    }

    // err diff is change in degrees in seconds from last update
    e[2] = ( e[0] - errLast ) / elapsed;

    // setup features (PID values) for motor A
    error = e[0] * k[0] +     // P component, raw error offset
            e[1] * k[1] +     // I component, total error
            e[2] * k[2];      // D component, rate of change of error

    // store last error values
    errLast = e[0];

    // store time of this update
    lastUpdated = micros;

    return error;
}

void DrogonPid::reset( unsigned long micros ) {
    e[0] = 0.0;
    e[1] = 0.0;
    e[2] = 0.0;

    errLast = 0.0;

    error = 0.0;

    lastUpdated = micros;
}

void DrogonPid::set_max_sum( double maxSum ) {
    this->maxSum = maxSum;
}

void DrogonPid::set_thetas( double kp, double ki, double kd ) {
    this->k[0] = kp;
    this->k[1] = ki;
    this->k[2] = kd;
}

double* DrogonPid::get_thetas() {
    return this->k;
}

double* DrogonPid::get_errors() {
    return this->e;
}

