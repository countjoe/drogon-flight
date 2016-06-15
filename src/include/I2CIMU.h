/*
 * I2CIMU.h
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
#ifndef __I2CIMU_H__
#define __I2CIMU_H__

#include "I2C.h"
#include "DrogonCommon.h"

#define LSM303_ADDRESS_ACCEL          (0x32 >> 1)         // 0011001x
#define LSM303_ADDRESS_MAG            (0x3C >> 1)         // 0011110x

#define L3GD20_ADDRESS                (0x6B)        // 1101011
#define L3GD20_POLL_TIMEOUT           (100)         // Maximum number of read attempts
#define L3GD20_ID                     0xD4
#define L3GD20H_ID                    0xD7

#define L3GD20_SENSITIVITY_250DPS  (0.00875F)      // Roughly 22/256 for fixed point match
#define L3GD20_SENSITIVITY_500DPS  (0.0175F)       // Roughly 45/256
#define L3GD20_SENSITIVITY_2000DPS (0.070F)        // Roughly 18/256
#define L3GD20_DPS_TO_RADS         (0.017453293F)  // degress/s to rad/s multiplier


typedef enum
{                                                     // DEFAULT    TYPE
  LSM303_REGISTER_ACCEL_CTRL_REG1_A         = 0x20,   // 00000111   rw
  LSM303_REGISTER_ACCEL_CTRL_REG2_A         = 0x21,   // 00000000   rw
  LSM303_REGISTER_ACCEL_CTRL_REG3_A         = 0x22,   // 00000000   rw
  LSM303_REGISTER_ACCEL_CTRL_REG4_A         = 0x23,   // 00000000   rw
  LSM303_REGISTER_ACCEL_CTRL_REG5_A         = 0x24,   // 00000000   rw
  LSM303_REGISTER_ACCEL_CTRL_REG6_A         = 0x25,   // 00000000   rw
  LSM303_REGISTER_ACCEL_REFERENCE_A         = 0x26,   // 00000000   r
  LSM303_REGISTER_ACCEL_STATUS_REG_A        = 0x27,   // 00000000   r
  LSM303_REGISTER_ACCEL_OUT_X_L_A           = 0x28,
  LSM303_REGISTER_ACCEL_OUT_X_H_A           = 0x29,
  LSM303_REGISTER_ACCEL_OUT_Y_L_A           = 0x2A,
  LSM303_REGISTER_ACCEL_OUT_Y_H_A           = 0x2B,
  LSM303_REGISTER_ACCEL_OUT_Z_L_A           = 0x2C,
  LSM303_REGISTER_ACCEL_OUT_Z_H_A           = 0x2D,
  LSM303_REGISTER_ACCEL_FIFO_CTRL_REG_A     = 0x2E,
  LSM303_REGISTER_ACCEL_FIFO_SRC_REG_A      = 0x2F,
  LSM303_REGISTER_ACCEL_INT1_CFG_A          = 0x30,
  LSM303_REGISTER_ACCEL_INT1_SOURCE_A       = 0x31,
  LSM303_REGISTER_ACCEL_INT1_THS_A          = 0x32,
  LSM303_REGISTER_ACCEL_INT1_DURATION_A     = 0x33,
  LSM303_REGISTER_ACCEL_INT2_CFG_A          = 0x34,
  LSM303_REGISTER_ACCEL_INT2_SOURCE_A       = 0x35,
  LSM303_REGISTER_ACCEL_INT2_THS_A          = 0x36,
  LSM303_REGISTER_ACCEL_INT2_DURATION_A     = 0x37,
  LSM303_REGISTER_ACCEL_CLICK_CFG_A         = 0x38,
  LSM303_REGISTER_ACCEL_CLICK_SRC_A         = 0x39,
  LSM303_REGISTER_ACCEL_CLICK_THS_A         = 0x3A,
  LSM303_REGISTER_ACCEL_TIME_LIMIT_A        = 0x3B,
  LSM303_REGISTER_ACCEL_TIME_LATENCY_A      = 0x3C,
  LSM303_REGISTER_ACCEL_TIME_WINDOW_A       = 0x3D
} lsm303AccelRegisters_t;

typedef enum
{
  LSM303_REGISTER_MAG_CRA_REG_M             = 0x00,
  LSM303_REGISTER_MAG_CRB_REG_M             = 0x01,
  LSM303_REGISTER_MAG_MR_REG_M              = 0x02,
  LSM303_REGISTER_MAG_OUT_X_H_M             = 0x03,
  LSM303_REGISTER_MAG_OUT_X_L_M             = 0x04,
  LSM303_REGISTER_MAG_OUT_Z_H_M             = 0x05,
  LSM303_REGISTER_MAG_OUT_Z_L_M             = 0x06,
  LSM303_REGISTER_MAG_OUT_Y_H_M             = 0x07,
  LSM303_REGISTER_MAG_OUT_Y_L_M             = 0x08,
  LSM303_REGISTER_MAG_SR_REG_Mg             = 0x09,
  LSM303_REGISTER_MAG_IRA_REG_M             = 0x0A,
  LSM303_REGISTER_MAG_IRB_REG_M             = 0x0B,
  LSM303_REGISTER_MAG_IRC_REG_M             = 0x0C,
  LSM303_REGISTER_MAG_TEMP_OUT_H_M          = 0x31,
  LSM303_REGISTER_MAG_TEMP_OUT_L_M          = 0x32
} lsm303MagRegisters_t;


/*=========================================================================
  MAGNETOMETER GAIN SETTINGS
  -----------------------------------------------------------------------*/
typedef enum
{
  LSM303_MAGGAIN_1_3                        = 0x20,  // +/- 1.3
  LSM303_MAGGAIN_1_9                        = 0x40,  // +/- 1.9
  LSM303_MAGGAIN_2_5                        = 0x60,  // +/- 2.5
  LSM303_MAGGAIN_4_0                        = 0x80,  // +/- 4.0
  LSM303_MAGGAIN_4_7                        = 0xA0,  // +/- 4.7
  LSM303_MAGGAIN_5_6                        = 0xC0,  // +/- 5.6
  LSM303_MAGGAIN_8_1                        = 0xE0   // +/- 8.1
} lsm303MagGain;	
/*=========================================================================*/

/*=========================================================================
  MAGNETOMETER UPDATE RATE SETTINGS
  -----------------------------------------------------------------------*/
typedef enum
{
  LSM303_MAGRATE_0_7                        = 0x00,  // 0.75 Hz
  LSM303_MAGRATE_1_5                        = 0x01,  // 1.5 Hz
  LSM303_MAGRATE_3_0                        = 0x62,  // 3.0 Hz
  LSM303_MAGRATE_7_5                        = 0x03,  // 7.5 Hz
  LSM303_MAGRATE_15                         = 0x04,  // 15 Hz
  LSM303_MAGRATE_30                         = 0x05,  // 30 Hz
  LSM303_MAGRATE_75                         = 0x06,  // 75 Hz
  LSM303_MAGRATE_220                        = 0x07   // 200 Hz
} lsm303MagRate;	
/*=========================================================================*/



typedef enum
{                                               // DEFAULT    TYPE
  L3GD20_REGISTER_WHO_AM_I            = 0x0F,   // 11010100   r
  L3GD20_REGISTER_CTRL_REG1           = 0x20,   // 00000111   rw
  L3GD20_REGISTER_CTRL_REG2           = 0x21,   // 00000000   rw
  L3GD20_REGISTER_CTRL_REG3           = 0x22,   // 00000000   rw
  L3GD20_REGISTER_CTRL_REG4           = 0x23,   // 00000000   rw
  L3GD20_REGISTER_CTRL_REG5           = 0x24,   // 00000000   rw
  L3GD20_REGISTER_REFERENCE           = 0x25,   // 00000000   rw
  L3GD20_REGISTER_OUT_TEMP            = 0x26,   //            r
  L3GD20_REGISTER_STATUS_REG          = 0x27,   //            r
  L3GD20_REGISTER_OUT_X_L             = 0x28,   //            r
  L3GD20_REGISTER_OUT_X_H             = 0x29,   //            r
  L3GD20_REGISTER_OUT_Y_L             = 0x2A,   //            r
  L3GD20_REGISTER_OUT_Y_H             = 0x2B,   //            r
  L3GD20_REGISTER_OUT_Z_L             = 0x2C,   //            r
  L3GD20_REGISTER_OUT_Z_H             = 0x2D,   //            r
  L3GD20_REGISTER_FIFO_CTRL_REG       = 0x2E,   // 00000000   rw
  L3GD20_REGISTER_FIFO_SRC_REG        = 0x2F,   //            r
  L3GD20_REGISTER_INT1_CFG            = 0x30,   // 00000000   rw
  L3GD20_REGISTER_INT1_SRC            = 0x31,   //            r
  L3GD20_REGISTER_TSH_XH              = 0x32,   // 00000000   rw
  L3GD20_REGISTER_TSH_XL              = 0x33,   // 00000000   rw
  L3GD20_REGISTER_TSH_YH              = 0x34,   // 00000000   rw
  L3GD20_REGISTER_TSH_YL              = 0x35,   // 00000000   rw
  L3GD20_REGISTER_TSH_ZH              = 0x36,   // 00000000   rw
  L3GD20_REGISTER_TSH_ZL              = 0x37,   // 00000000   rw
  L3GD20_REGISTER_INT1_DURATION       = 0x38    // 00000000   rw
} l3gd20Registers_t;

typedef enum
{
  L3DS20_RANGE_250DPS,
  L3DS20_RANGE_500DPS,
  L3DS20_RANGE_2000DPS
} l3gd20Range_t;



class I2CLSM303Accel {
  public:
    I2CLSM303Accel(I2C* i2c);
    
    void read(vector3d* data);

  private:
    I2C* i2c;
};

class I2CLSM303Mag {
  public:
    I2CLSM303Mag(I2C* i2c);
    
    void set_gain(lsm303MagGain gain);
    void read(vector3d* data);

  private:
    I2C* i2c;
};

class I2CL3GD20Gyro {
  public:
    I2CL3GD20Gyro(I2C* i2c);

    void set_range(l3gd20Range_t range);
    void read(vector3d* data);

  private:
    I2C* i2c;
    l3gd20Range_t range;
};

#endif // __I2CIMU_H__

