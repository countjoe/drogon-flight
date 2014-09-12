/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_joemonti_drogon_flight_DrogonFlight */

#ifndef _Included_org_joemonti_drogon_flight_DrogonFlight
#define _Included_org_joemonti_drogon_flight_DrogonFlight
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_joemonti_drogon_flight_DrogonFlight
 * Method:    setPosition
 * Signature: (DD)V
 */
JNIEXPORT void JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_setPosition
  (JNIEnv *, jobject, jdouble, jdouble);

/*
 * Class:     org_joemonti_drogon_flight_DrogonFlight
 * Method:    positionUpdate
 * Signature: (J[D[[D)V
 */
JNIEXPORT void JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_positionUpdate
  (JNIEnv *, jobject, jlong, jdoubleArray, jdoubleArray);

/*
 * Class:     org_joemonti_drogon_flight_DrogonFlight
 * Method:    getPositionX
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_getPositionX
  (JNIEnv *, jobject);

/*
 * Class:     org_joemonti_drogon_flight_DrogonFlight
 * Method:    getPositionY
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_getPositionY
  (JNIEnv *, jobject);

/*
 * Class:     org_joemonti_drogon_flight_DrogonFlight
 * Method:    controlUpdate
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_controlUpdate
  (JNIEnv *, jobject, jlong);

/*
 * Class:     org_joemonti_drogon_flight_DrogonFlight
 * Method:    updateThetas
 * Signature: (DDD)V
 */
JNIEXPORT void JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_updateThetas
  (JNIEnv *, jobject, jdouble, jdouble, jdouble);

/*
 * Class:     org_joemonti_drogon_flight_DrogonFlight
 * Method:    getMotorAdjust
 * Signature: (I)D
 */
JNIEXPORT jdouble JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_getMotorAdjust
  (JNIEnv *, jobject, jint);

/*
 * Class:     org_joemonti_drogon_flight_DrogonFlight
 * Method:    getPidErrorA
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_getPidErrorA
  (JNIEnv *, jobject);

/*
 * Class:     org_joemonti_drogon_flight_DrogonFlight
 * Method:    getPidErrorB
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_getPidErrorB
  (JNIEnv *, jobject);

/*
 * Class:     org_joemonti_drogon_flight_DrogonFlight
 * Method:    getMotorOffsetA
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_getMotorOffsetA
  (JNIEnv *, jobject);

/*
 * Class:     org_joemonti_drogon_flight_DrogonFlight
 * Method:    getMotorOffsetB
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_getMotorOffsetB
  (JNIEnv *, jobject);

/*
 * Class:     org_joemonti_drogon_flight_DrogonFlight
 * Method:    getMicros
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_org_joemonti_drogon_flight_DrogonFlight_getMicros
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
