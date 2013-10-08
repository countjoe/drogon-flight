/*
 * DrogonPid.h
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

#ifndef __DROGONPID_H__
#define __DROGONPID_H__

class DrogonPid {
    public:
        DrogonPid( double kp, double ki, double kd );

        double update( unsigned long micros, double value );

        void reset( unsigned long micros );

        void set_thetas( double kp, double ki, double kd );

        void set_max_sum( double maxSum );

        double ep;
        double ei;
        double ed;

        double error;

    private:
        double kp;
        double ki;
        double kd;

        double errLast;

        unsigned long lastUpdated;

        double maxSum;
};


#endif /* __DROGONPID_H__ */
