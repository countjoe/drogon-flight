#include "org_joemonti_drogon_flight_DrogonFlight.h"

#include "arduino/DrogonController.h"

#include <iostream>
using namespace std;

#include <sys/time.h>

DrogonPosition position;
DrogonController dc(&position);

double zeroPosition[] = { 0.0, 0.0, 0.0 };

JNIEXPORT void JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_setPosition
  (JNIEnv *env, jobject obj, jdouble x, jdouble y) 
{
	position.x = x;
	position.y = y;
}


JNIEXPORT void JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_positionUpdate
  (JNIEnv *env, jobject obj, jlong micros, jdoubleArray accel, jdoubleArray gyro)
{
	double accelValues[3];
	double gyroValues[3];
	
	jdouble *accelEls = env->GetDoubleArrayElements(accel, 0);
	jdouble *gyroEls = env->GetDoubleArrayElements(gyro, 0);
	
	accelValues[0] = accelEls[0];
	accelValues[1] = accelEls[1];
	accelValues[2] = accelEls[2];
	
	gyroValues[0] = gyroEls[0];
	gyroValues[1] = gyroEls[1];
	gyroValues[2] = gyroEls[2];
	
	position.update( micros, accelValues, gyroValues );

	env->ReleaseDoubleArrayElements(accel, accelEls, 0);
	env->ReleaseDoubleArrayElements(gyro, gyroEls, 0);
}


JNIEXPORT jdouble JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_getPositionX
  (JNIEnv *env, jobject obj)
{
	return position.x;
}

JNIEXPORT jdouble JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_getPositionY
  (JNIEnv *env, jobject obj)
{
	return position.y;
}

JNIEXPORT void JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_controlUpdate
  (JNIEnv *env, jobject obj, jlong micros)
{
	dc.control_update(micros, zeroPosition);
}

JNIEXPORT void JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_updateThetas
  (JNIEnv *env, jobject obj, jdouble kp, jdouble ki, jdouble kd)
{
	dc.pidA.set_thetas( kp, ki, kd );
	dc.pidB.set_thetas( kp, ki, kd );
}

JNIEXPORT jdouble JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_getMotorAdjust
  (JNIEnv *env, jobject obj, jint idx)
{
	return dc.motorAdjusts[idx];
}

JNIEXPORT jdouble JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_getPidErrorA
  (JNIEnv *env, jobject obj)
{
	return dc.pidA.error;
}

JNIEXPORT jdouble JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_getPidErrorB
  (JNIEnv *env, jobject obj)
{
	return dc.pidB.error;
}

JNIEXPORT jdouble JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_getMotorOffsetA
  (JNIEnv *env, jobject obj)
{
	return dc.motorOffsetA;
}

JNIEXPORT jdouble JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_getMotorOffsetB
  (JNIEnv *env, jobject obj)
{
	return dc.motorOffsetB;
}

JNIEXPORT jlong JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_getMicros
  (JNIEnv *env, jobject obj)
{
	struct timeval tv;

	gettimeofday( &tv, NULL );
	
	return 1000000 * tv.tv_sec + tv.tv_usec;
}

