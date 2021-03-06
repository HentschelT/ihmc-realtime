/*
 *   Copyright 2014 Florida Institute for Human and Machine Cognition (IHMC)
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 *
 *    Written by Jesper Smith with assistance from IHMC team members
 */

#include "Affinity.h"
#include <sched.h>
#include <pthread.h>
#include <sched.h>
#include "Utils.hpp"
#include "Thread.h"

/*
 * Class:     us_ihmc_affinity_Affinity
 * Method:    setAffinity
 * Signature: ([I)V
 */
JNIEXPORT void JNICALL Java_us_ihmc_affinity_Affinity_setAffinity___3I
  (JNIEnv *env, jclass, jintArray jcpus)
{
	jsize arrayLength = env->GetArrayLength(jcpus);
	jint *cpus = env->GetIntArrayElements(jcpus, 0);
#ifndef __MACH__
	cpu_set_t set;
	CPU_ZERO(&set);

	for(int i = 0; i < arrayLength; i++)
	{
		CPU_SET(cpus[i], &set);
	}

	if(sched_setaffinity(0, sizeof(set), &set) == -1)
	{
		throwRuntimeException(env, "sched_setaffinity: Cannot set processor affinity. Make sure that the CPUS exist");
	}
#else
    thread_port_t port = pthread_mach_thread_np(pthread_self());
    struct thread_affinity_policy policy;

    policy.affinity_tag = cpus[0];

    int ret = thread_policy_set(port, THREAD_AFFINITY_POLICY, (thread_policy_t) &policy, THREAD_AFFINITY_POLICY_COUNT);

    if(ret != 0)
    {
    throwRuntimeException(env, "thread_policy_set: Error setting processor affinity.");
    }
#endif
}

/*
 * Class:     us_ihmc_affinity_Affinity
 * Method:    setAffinity
 * Signature: (J[I)V
 */
JNIEXPORT void JNICALL Java_us_ihmc_affinity_Affinity_setAffinity__J_3I
  (JNIEnv *env, jclass, jlong threadID, jintArray jcpus)
{
    Thread* thread = (Thread*) threadID;
    jsize arrayLength = env->GetArrayLength(jcpus);
    jint *cpus = env->GetIntArrayElements(jcpus, 0);

#ifndef __MACH__
	cpu_set_t set;
	CPU_ZERO(&set);

	for(int i = 0; i < arrayLength; i++)
	{
		CPU_SET(cpus[i], &set);
	}

	if(pthread_setaffinity_np(thread->thread, sizeof(set), &set) == -1)
	{
		throwRuntimeException(env, "pthread_setaffinity_np: Cannot set processor affinity. Make sure that the CPUS exist");
	}
#else
    thread_port_t port = pthread_mach_thread_np(thread->thread);
    struct thread_affinity_policy policy;

    policy.affinity_tag = cpus[0];

    int ret = thread_policy_set(port, THREAD_AFFINITY_POLICY, (thread_policy_t) &policy, THREAD_AFFINITY_POLICY_COUNT);

    if(ret != 0)
    {
    throwRuntimeException(env, "thread_policy_set: Error setting processor affinity.");
    }
#endif
}
