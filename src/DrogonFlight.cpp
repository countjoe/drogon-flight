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

#include "DrogonFlight.h"

#include <iostream>
#include <string>
#include <time.h>
#include <stdio.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

using namespace std;


DrogonFlight::DrogonFlight() : ctrl(&pos), rcore("localhost") // , accel(&i2c), mag(&i2c), gyro(&i2c)
{
    zero_vector3d(&accelValues);
    zero_vector3d(&magValues);
    zero_vector3d(&gyroValues);
    
    motorMaster = 0.0;
    motorRotate[0] = 0.0;
    motorRotate[1] = 0.0;
    motorRotate[2] = 0.0;
}

DrogonFlight::~DrogonFlight()
{
    rcore.close();
    //i2c.close();
}

void DrogonFlight::run()
{
    //FILE* f;
    //char fname[100];

    double t = now();

    //sprintf(fname, "imu.%ld.log", (long)(t*1000.0));
    //f = fopen(fname, "w");
    //chrono::high_resolution_clock::time_point now_tp = chrono::high_resolution_clock::now();
    //chrono::high_resolution_clock::time_point last_tp = chrono::high_resolution_clock::now();
    //chrono::milliseconds log_interval(250);
    chrono::milliseconds update_interval(20);

    while (true) {
        t = now();

        /*
        read_imu();
        pos.update(t, &accelValues, &gyroValues);

        now_tp = chrono::high_resolution_clock::now();
        if ( (now_tp - last_tp) > log_interval ) {
            //printf("%f,%f,%f,%f,%f,%f,%f\n", t, accelValues.x, accelValues.y, gyroValues.x, gyroValues.y, pos.position.x, pos.position.y);
            last_tp = chrono::high_resolution_clock::now();
        }
        */

        if ( rcore.read() ) {
            if ( rcore.is_arm_data() ) {
                uint8_t arm = rcore.get_arm_data();
                printf("%f,ARM,%d\n", t, arm);
            } else if ( rcore.is_motor_data() ) {
                double motor = rcore.get_motor_data();
                printf("%f,MOTOR,%.12f\n", t, motor);
            }
        }
                
        this_thread::sleep_for(update_interval);
    }

    //fclose(f);
}

void DrogonFlight::read_imu()
{
    //accel.read(&accelValues);

    //mag.read(&magValues);

    //gyro.read(&gyroValues);
}


void DrogonFlight::print_vec(FILE* f, vector3d* vec)
{
    fprintf(f, ",%f,%f,%f", vec->x, vec->y, vec->z);
}

double DrogonFlight::now()
{
    chrono::high_resolution_clock::time_point now_tp = chrono::high_resolution_clock::now();
    return now_tp.time_since_epoch().count() * chrono::high_resolution_clock::period::num / static_cast<double>(chrono::high_resolution_clock::period::den);
}