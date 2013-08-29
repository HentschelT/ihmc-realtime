#include <time.h>
#include <jni.h>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>

const inline int NSEC_PER_SEC = 1000000000;


/**
 * Check if cond == true, otherwise throw a Java RuntimeException
 *
 * @param env Java environment
 * @Param
 */
#define JNIassert(env, cond) \
	do { \
		if(!(cond)) { \
			throwRuntimeException((env), __FILE__, __PRETTY_FUNCTION__, __LINE__); \
		} \
	} while(0)


/**
 * Attach the current thread to the VM if necessary and return the Java environment
 *
 * @param vm Java VM
 */
inline JNIEnv* getEnv(JavaVM* vm)
{
	// Get the java environment
	JNIEnv* env;
	int getEnvStat = vm->GetEnv((void **) &env,
			JNI_VERSION_1_6);
	if (getEnvStat == JNI_EDETACHED)
	{

		struct sched_param priority;
		int policy;
		pthread_t thread = pthread_self();

		JNIassert(env, pthread_getschedparam(pthread_self(), &policy, &priority) == 0);

		std::cout << "Attaching native thread " << thread << " with priority " << priority << " to JVM" << std::endl;
		if (vm->AttachCurrentThread((void **) &env, NULL)
				!= 0)
		{
			std::cerr << "Failed to attach" << std::endl;
			return 0;
		}

	}
	else if (getEnvStat == JNI_EVERSION)
	{
		std::cerr << "GetEnv: Version not supported" << std::endl;
		return 0;
	}
	else if (getEnvStat == JNI_OK)
	{
		//
	}

	return env;
}

/**
 * Helper function to throw a Java runtime exception
 *
 * @param env Java environment
 * @param msg Message to throw
 */
inline void throwRuntimeException(JNIEnv* env, std::string msg)
{
	jclass exClass = env->FindClass("java/lang/RuntimeException");
	env->ThrowNew(exClass, msg.c_str());
}

/**
 * Helper function to throw a formatted runtime exception
 *
 * @param env Java environment
 * @param file Caller file name
 * @param function Caller function name
 * @param line Caller line #
 */
inline void throwRuntimeException(JNIEnv* env, std::string file, std::string function, int line)
{
	throwRuntimeException(env, std::string("Exception in ") + file + std::string(", ") + function + std::string(" at line ") + line);
}


/**
 * Normalize ts to nsec < 1000000000
 *
 * @param ts
 */
static inline void tsnorm(struct timespec *ts)
{
   while (ts->tv_nsec >= NSEC_PER_SEC) {
      ts->tv_nsec -= NSEC_PER_SEC;
      ts->tv_sec++;
   }
}

/**
 * Add ts2 to ts1 and set ts1 to the result
 *
 * @param ts1
 * @param ts2
 */
static inline void tsadd(struct timespec *ts1, struct timespec *ts2)
{
	ts1->tv_sec += ts2->tv_sec;
	ts1->tv_nsec += ts2->tv_nsec;
	tsnorm(ts1);
}