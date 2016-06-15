/*
 * DrogonFlight.cpp
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

#include <iostream>
#include <string>
#include <time.h>
#include <stdio.h>
#include <thread>         // std::this_thread::sleep_for

#include "DrogonFlight.h"
#include "DrogonConstants.h"

using namespace std;

double map_double(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

DrogonFlight::DrogonFlight() : ctrl(&pos), rcore("localhost"), accel(&i2c), mag(&i2c), gyro(&i2c) //, motors(&i2c)
{
    zero_vector3d(&accelValues);
    zero_vector3d(&magValues);
    zero_vector3d(&gyroValues);

    armed = false;
    controlEngaged = false;
    
    motorMaster = 0.0;
    motorRotate[0] = 0.0;
    motorRotate[1] = 0.0;
    motorRotate[2] = 0.0;
}

DrogonFlight::~DrogonFlight()
{
    motors_disarm();
    rcore.close();
    i2c.close();
}

void DrogonFlight::run()
{
    //FILE* f;
    //char fname[100];

    double t = 0.0;

    //sprintf(fname, "imu.%ld.log", (long)(t*1000.0));
    //f = fopen(fname, "w");
    chrono::high_resolution_clock::time_point now_tp = chrono::high_resolution_clock::now();
    chrono::high_resolution_clock::time_point end_tp = chrono::high_resolution_clock::now();
    chrono::high_resolution_clock::time_point last_tp = chrono::high_resolution_clock::now();
    chrono::milliseconds log_interval(250);
    chrono::milliseconds update_interval(20);
    chrono::high_resolution_clock::duration sleep_time;
    chrono::milliseconds min_update_interval(2);

    while (true) {
        now_tp = chrono::high_resolution_clock::now();
        t = to_seconds(now_tp);

        read_rcore(t);

        read_imu();
        pos.update(t, &accelValues, &gyroValues);

        if (armed) {
            control_update(t);
            update_motors();
        }

        if ( (now_tp - last_tp) > log_interval ) {
            printf("%f,%f,%f,%f,%f,%f,%f,%d,%d,%d,%d\n", 
                t, 
                accelValues.x, accelValues.y, 
                gyroValues.x, gyroValues.y, 
                pos.position.x, pos.position.y,
                motorValues[0], motorValues[1], motorValues[2], motorValues[3]);
            last_tp = chrono::high_resolution_clock::now();
        }

        end_tp = chrono::high_resolution_clock::now();
        sleep_time = end_tp - now_tp;
        if ( sleep_time < min_update_interval ) {
            sleep_time = min_update_interval;
        }

        this_thread::sleep_for(sleep_time);
    }

    //fclose(f);
}

void DrogonFlight::read_imu()
{
    accel.read(&accelValues);

    mag.read(&magValues);

    gyro.read(&gyroValues);
}

void DrogonFlight::read_rcore(double t)
{
    if ( rcore.read() ) {
        if ( rcore.is_arm_data() ) {
            uint8_t arm = rcore.get_arm_data();
            if (arm == 0) {
                motors_disarm();
            } else {
                motors_arm();
            }
            printf("%f,ARM,%d\n", t, arm);
        } else if ( rcore.is_motor_data() ) {
            double motor = rcore.get_motor_data();
            motorMaster = motor;
            printf("%f,MOTOR,%.12f\n", t, motor);
        }
    }
}

void DrogonFlight::motors_arm()
{
    if (!armed) {
        motorValues[0] = 
            motorValues[1] = 
            motorValues[2] = 
            motorValues[3] = 
            motorValues[4] = MIN_MOTOR_VALUE;
        update_motors();
        armed = true;
    }
}

void DrogonFlight::motors_disarm()
{
    if (armed) {
        motorValues[0] = 
            motorValues[1] = 
            motorValues[2] = 
            motorValues[3] = 
            motorValues[4] = 0;
        update_motors();
        armed = false;
    }
}

void DrogonFlight::control_update(double t)
{
    int target = (int) map_double( motorMaster, 0.0, 100.0, MIN_MOTOR_VALUE, MAX_MOTOR_VALUE );
    target = constrain( target, MIN_MOTOR_VALUE, MAX_MOTOR_VALUE );

    if (controlEngaged) {
        if ( target < CONTROL_ENGAGE_THRESHOLD_LOW ) {
            //controller.tune();
            //log_pid( );
            ctrl.reset( t );
            //nextTuneTime = millis() + TUNER_FREQUENCY;

            motorAdjusts[0] = 0.0;
            motorAdjusts[1] = 0.0;
            motorAdjusts[2] = 0.0;
            motorAdjusts[3] = 0.0;

            zRotAdjust = 0.0;

            controlEngaged = false;
        } else {
            /*
            if ( millis() > nextTuneTime ) {
                ctrl.tune();
                log_pid( );
                nextTuneTime = millis() + TUNER_FREQUENCY;
            }
            */

            ctrl.control_update( t, motorRotate );

            motorAdjusts[0] = ctrl.motorAdjusts[0];
            motorAdjusts[1] = ctrl.motorAdjusts[1];
            motorAdjusts[2] = ctrl.motorAdjusts[2];
            motorAdjusts[3] = ctrl.motorAdjusts[3];

            zRotAdjust = ctrl.zRotAdjust;
        }
    } else if ( target >= CONTROL_ENGAGE_THRESHOLD_HIGH ) {
        ctrl.reset( t );
        //nextTuneTime = millis() + TUNER_FREQUENCY;

        ctrl.control_update( t, motorRotate );

        motorAdjusts[0] = ctrl.motorAdjusts[0];
        motorAdjusts[1] = ctrl.motorAdjusts[1];
        motorAdjusts[2] = ctrl.motorAdjusts[2];
        motorAdjusts[3] = ctrl.motorAdjusts[3];

        zRotAdjust = ctrl.zRotAdjust;

        controlEngaged = true;
    }

    motorAdjusts[0] = constrain( motorAdjusts[0], MIN_MOTOR_ADJUST, MAX_MOTOR_ADJUST );
    motorAdjusts[1] = constrain( motorAdjusts[1], MIN_MOTOR_ADJUST, MAX_MOTOR_ADJUST );
    motorAdjusts[2] = constrain( motorAdjusts[2], MIN_MOTOR_ADJUST, MAX_MOTOR_ADJUST );
    motorAdjusts[3] = constrain( motorAdjusts[3], MIN_MOTOR_ADJUST, MAX_MOTOR_ADJUST );

    zRotAdjust = max( MIN_MOTOR_ZROT_ADJUST, min( MAX_MOTOR_ZROT_ADJUST, zRotAdjust ) );

    motorValues[0] = constrain( target + motorAdjusts[0] + zRotAdjust, MIN_MOTOR_VALUE, MAX_MOTOR_VALUE );
    motorValues[1] = constrain( target + motorAdjusts[1] + zRotAdjust, MIN_MOTOR_VALUE, MAX_MOTOR_VALUE );
    motorValues[2] = constrain( target + motorAdjusts[2] + zRotAdjust, MIN_MOTOR_VALUE, MAX_MOTOR_VALUE );
    motorValues[3] = constrain( target + motorAdjusts[3] + zRotAdjust, MIN_MOTOR_VALUE, MAX_MOTOR_VALUE );
}

void DrogonFlight::update_motors() {
  //motors.setMicros( 0, motorValues[0] );
  //motors.setMicros( 1, motorValues[1] );
  //motors.setMicros( 2, motorValues[2] );
  //motors.setMicros( 3, motorValues[3] );
}

void DrogonFlight::print_vec(FILE* f, vector3d* vec)
{
    fprintf(f, ",%f,%f,%f", vec->x, vec->y, vec->z);
}

double DrogonFlight::to_seconds(chrono::high_resolution_clock::time_point now_tp)
{
    return now_tp.time_since_epoch().count() * chrono::high_resolution_clock::period::num / static_cast<double>(chrono::high_resolution_clock::period::den);
}