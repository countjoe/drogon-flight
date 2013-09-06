#ifndef __DROGON_POSITION_H__
#define __DROGON_POSITION_H__

#include <math.h>

#ifndef PI
#define PI M_PI
#endif

#define ACCEL_SCALE (1/6.2)

#define INIT_VAR_SQ 4
#define ACCEL_VAR_SQ 1.96
#define GYRO_VAR_SQ 1.96
#define VAR_UPDATE_SCALE 2.0

class DrogonPosition {
	public:
		DrogonPosition(void);
		
		void update( long micros, const double accelValues[3], const double gyroValues[3] );
		
		double x;
		double y;
	private:
		double calc_mean( double mean1, double var1, double mean2, double var2 );
		double calc_var( double var1, double var2 );
		
		double xVarSq;
		double yVarSq;
};

#endif
