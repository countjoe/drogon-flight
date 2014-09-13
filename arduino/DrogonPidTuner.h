/*
 * DrogonPidTuner.h
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

#ifndef __DROGONPIDTUNER_H__
#define __DROGONPIDTUNER_H__

#include "DrogonPid.h"

class DrogonPidTuner {
    public:
        DrogonPidTuner( DrogonPid *pid );

        void reset( void );

        void update( double error );

        void tune( void );

        double* getAdjusts( void );

        double getLastError( void );
    private:
        DrogonPid *pid;

        double errorTotal;
        int errorCount;

        double lastError;

        double dk[3];
        int dki;
        double lastDk;

        double bestError;
};


#endif /* __DROGONPIDTUNER_H__ */
