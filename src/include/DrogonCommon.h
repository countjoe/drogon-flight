/*
 * DrogonCommon.h
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

#ifndef __DROGON_COMMON_H__
#define __DROGON_COMMON_H__


typedef struct vector3d_s
{
    double x;
    double y;
    double z;
} vector3d;

inline void zero_vector3d(vector3d* vec) {
    vec->x = vec->y = vec->z = 0.0;
}

inline void update_vector3d_array(vector3d* vec, double values[3]) {
    vec->x = values[0];
    vec->y = values[1];
    vec->z = values[2];
}


typedef struct vector4d_s
{
    double m1;
    double m2;
    double m3;
    double m4;
} vector4d;

inline void zero_vecto43d(vector4d* vec) {
    vec->m1 = vec->m2 = vec->m3 = vec->m4 = 0.0;
}

inline void update_vector4d_array(vector4d* vec, double values[4]) {
    vec->m1 = values[0];
    vec->m2 = values[1];
    vec->m3 = values[2];
    vec->m4 = values[4];
}

#endif  // __DROGON_COMMON_H__