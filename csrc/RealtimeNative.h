/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class us_ihmc_realtime_RealtimeNative */

#ifndef _Included_us_ihmc_realtime_RealtimeNative
#define _Included_us_ihmc_realtime_RealtimeNative
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     us_ihmc_realtime_RealtimeNative
 * Method:    mlockall
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_us_ihmc_realtime_RealtimeNative_mlockall
  (JNIEnv *, jclass);

/*
 * Class:     us_ihmc_realtime_RealtimeNative
 * Method:    registerVM
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_us_ihmc_realtime_RealtimeNative_registerVM
  (JNIEnv *, jclass);

/*
 * Class:     us_ihmc_realtime_RealtimeNative
 * Method:    createThread
 * Signature: (Ljava/lang/Runnable;IZZJJJJ)J
 */
JNIEXPORT jlong JNICALL Java_us_ihmc_realtime_RealtimeNative_createThread
  (JNIEnv *, jclass, jobject, jint, jboolean, jboolean, jlong, jlong, jlong, jlong);

/*
 * Class:     us_ihmc_realtime_RealtimeNative
 * Method:    startThread
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_us_ihmc_realtime_RealtimeNative_startThread
  (JNIEnv *, jclass, jlong);

/*
 * Class:     us_ihmc_realtime_RealtimeNative
 * Method:    join
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_us_ihmc_realtime_RealtimeNative_join
  (JNIEnv *, jclass, jlong);

/*
 * Class:     us_ihmc_realtime_RealtimeNative
 * Method:    waitForNextPeriod
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_us_ihmc_realtime_RealtimeNative_waitForNextPeriod
  (JNIEnv *, jclass, jlong);

/*
 * Class:     us_ihmc_realtime_RealtimeNative
 * Method:    waitUntil
 * Signature: (JJJ)J
 */
JNIEXPORT jlong JNICALL Java_us_ihmc_realtime_RealtimeNative_waitUntil
  (JNIEnv *, jclass, jlong, jlong, jlong);

/*
 * Class:     us_ihmc_realtime_RealtimeNative
 * Method:    setNextPeriodToClock
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_us_ihmc_realtime_RealtimeNative_setNextPeriodToClock
  (JNIEnv *, jclass, jlong);

/*
 * Class:     us_ihmc_realtime_RealtimeNative
 * Method:    setNextPeriod
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_us_ihmc_realtime_RealtimeNative_setNextPeriod
  (JNIEnv *, jclass, jlong, jlong, jlong);

/*
 * Class:     us_ihmc_realtime_RealtimeNative
 * Method:    getNextPeriod
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_us_ihmc_realtime_RealtimeNative_getNextPeriod
  (JNIEnv *, jclass, jlong);

/*
 * Class:     us_ihmc_realtime_RealtimeNative
 * Method:    getMaximumPriorityNative
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_us_ihmc_realtime_RealtimeNative_getMaximumPriorityNative
  (JNIEnv *, jclass);

/*
 * Class:     us_ihmc_realtime_RealtimeNative
 * Method:    getMinimumPriorityNative
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_us_ihmc_realtime_RealtimeNative_getMinimumPriorityNative
  (JNIEnv *, jclass);

/*
 * Class:     us_ihmc_realtime_RealtimeNative
 * Method:    getCurrentTimeNative
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_us_ihmc_realtime_RealtimeNative_getCurrentTimeNative
  (JNIEnv *, jclass);

/*
 * Class:     us_ihmc_realtime_RealtimeNative
 * Method:    getCurrentThreadPriority
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_us_ihmc_realtime_RealtimeNative_getCurrentThreadPriority
  (JNIEnv *, jclass);

/*
 * Class:     us_ihmc_realtime_RealtimeNative
 * Method:    getCurrentThreadScheduler
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_us_ihmc_realtime_RealtimeNative_getCurrentThreadScheduler
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
