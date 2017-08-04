/*
 * I2CLidar.h
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
#ifndef __I2CLIDAR_H__
#define __I2CLIDAR_H__

#include <chrono>

#include "I2C.h"
#include "DrogonCommon.h"

#define LIDAR_ADDRESS 0x62
#define LIDAR_REG_COMMAND 0x00
#define LIDAR_REG_STATUS 0x47
#define LIDAR_REG_DATA_H 0x0F
#define LIDAR_REG_DATA_L 0x10

#define LIDAR_CMD_ACQUIRE 0x04

#define LIDAR_STATE_NONE           0
#define LIDAR_STATE_INITIALIZING   1
#define LIDAR_STATE_ACQUIRING      2
#define LIDAR_STATE_READY          3

#define LIDAR_STAT_BUSY         0x01
#define LIDAR_STAT_REF_OVERFLOW 0x02
#define LIDAR_STAT_SIG_OVERFLOW 0x04
#define LIDAR_STAT_PIN_STATUS   0x08
#define LIDAR_STAT_SECOND_PEAK  0x10
#define LIDAR_STAT_TIMESTAMP    0x20
#define LIDAR_STAT_INVALID      0x40
#define LIDAR_STAT_EYE_SAFE     0x80

class I2CLidar {
public:
    I2CLidar(I2C* i2c);
    
    int read();

private:
    I2C* i2c;
    int state;
    std::chrono::high_resolution_clock::time_point state_start;
    int value;

    std::chrono::milliseconds initializing_interval;
    std::chrono::milliseconds acquiring_interval;
    std::chrono::milliseconds ready_interval;
    std::chrono::milliseconds retry_interval;

    void internal_initialize();
    void internal_acquire();
    void internal_read();
};

#endif // __I2CLIDAR_H__

/*

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/types.h>
#include <time.h>
#include <chrono>
#include <thread>

using namespace std;

int main(int argc, char** argv) {
    chrono::milliseconds init_time(100);
    chrono::milliseconds sleep_time(20);
    
    cout << "HEllo WOrld" << endl;
    
    int file = open("/dev/i2c-1", O_RDWR);
    int tmp;
    
    tmp = ioctl(file, I2C_SLAVE, 0x62);
    if (tmp < 0) {
        cout << "Error setting slave addr: " << tmp << endl;
        return 1;
    }
    
    tmp = i2c_smbus_read_byte_data(file, 0x47);
    if (tmp < 0) {
        cout << "Error reading data: " << tmp << endl;
        return 1;
    }
    cout << "status: " << hex << tmp << endl;
    
    this_thread::sleep_for(init_time);
    
    tmp = i2c_smbus_write_byte_data(file, 0x00, 0x04);
    if (tmp < 0) {
        cout << "Error writing mode: " << tmp << endl;
        return 1;
    }

    this_thread::sleep_for(sleep_time);
    
    for ( int i = 0; i < 10 && tmp < 0; i++ ) {
        tmp = i2c_smbus_read_byte_data(file, 0x0f);
        if (tmp < 0) this_thread::sleep_for(sleep_time);
    }
    if (tmp < 0) {
        cout << "Error reading data: " << tmp << endl;
        return 1;
    }
    int b0 = tmp;
    cout << "   high: " << b0 << endl;
    tmp = i2c_smbus_read_byte_data(file, 0x10);
    if (tmp < 0) {
        cout << "Error reading data: " << tmp << endl;
        return 1;
    }
    int b1 = tmp;
    cout << "    low: " << b1 << endl;
    long data = (int16_t)( b1 | ((int16_t)b0 << 8) );
    cout << "Read value: " << dec << data << endl;

    close(file);
    
    return 0;
}

*/