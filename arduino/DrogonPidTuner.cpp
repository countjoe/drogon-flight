/*
 * DrogonPidTuner.cpp
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

#include "DrogonPidTuner.h"

#include "DrogonConstants.h"

#include <stdlib.h>

DrogonPidTuner::DrogonPidTuner( DrogonPid *pid ) {
    this->pid = pid;

    this->errorTotal = 0.0;
    this->errorCount = 0;

    this->firstPass = true;

    this->dk[0] = TUNER_INIT;
    this->dk[1] = TUNER_INIT;
    this->dk[2] = TUNER_INIT;

    this->dki = 0;
    this->lastDk = 0.0;

    this->lastError = 0.0;
    this->bestError = 10000000.0;
}

void DrogonPidTuner::reset( void ) {
    this->dki = 0;

    this->lastDk = 0.0;

    this->errorTotal = 0.0;
    this->errorCount = 0;
}

void DrogonPidTuner::update( double error ) {
    if ( this->bestError < TUNER_TOLERANCE ) return;

    this->errorTotal += abs(error);
    this->errorCount += 1;
}

void DrogonPidTuner::tune( void ) {
    if ( this->bestError < TUNER_TOLERANCE ) return;
    if ( this->errorCount == 0 ) return;

    // base on average error
    double errorAvg = this->errorTotal / this->errorCount;

    if ( this->firstPass ) {
        this->bestError = errorAvg;
        this->firstPass = false;
    } else if ( errorAvg < this->bestError ) {
        // yay!
        this->bestError = errorAvg;

        // scale dk[i], a little less if increasing, a little more if decreasing
        this->dk[this->dki] *= ( this->dk[this->dki] > 0.0 ? TUNER_SCALE_MINOR : TUNER_SCALE_MAJOR );

        this->dki += 1;
        if ( this->dki >= 3 ) { // be sure to wrap around
            this->dki = 0;
        }
    } else {
        // oops! undo last change :(
        this->pid->get_thetas()[this->dki] -= this->lastDk;

        if ( this->dk[this->dki] > 0.0 ) {
            // reverse direction
            this->dk[this->dki] *= -1.0;
        } else {
            // reverse direction and decrease dk[i]
            this->dk[this->dki] *= -TUNER_SCALE_DOWN;
            // move on to next constant
            this->dki += 1;
            if ( this->dki >= 3 ) { // be sure to wrap around
                this->dki = 0;
            }
        }
    }

    // scale by random number between 0.5 and 1.0 to introduce some uncertainty
    double dk = this->dk[this->dki]; // * ( ( (rand()/(double)RAND_MAX) / 2.0 ) + 0.5 );

    this->pid->get_thetas()[this->dki] += dk;

    this->lastDk = dk;

    this->errorTotal = 0.0;
    this->errorCount = 0;

    this->lastError = errorAvg;
}

double* DrogonPidTuner::getAdjusts( void ) {
    return this->dk;
}

double DrogonPidTuner::getLastError( void ) {
    return this->lastError;
}

double DrogonPidTuner::getBestError( void ) {
    return this->bestError;
}
