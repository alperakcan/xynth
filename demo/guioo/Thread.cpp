#include <time.h>
#include <pthread.h>
#include <sys/signal.h>
#include <sys/time.h>

#include "Thread.h"

////////////////////////////////////////////////////////////////////////////////
//
Thread::Thread()
{
	threadHandle = 0;
	stopRequest = false;
	stopComplete = false;
}

Thread::~Thread()
{
}

////////////////////////////////////////////////////////////////////////////////
//
bool Thread::sleep(int msec)
{
	timespec sleepTime =
	{ msec / 1000, msec % 1000 * 1000000 };
	return nanosleep(&sleepTime, NULL) == 0;
}

////////////////////////////////////////////////////////////////////////////////
// override this to implement your worker thread
//
void Thread::run()
{
}

////////////////////////////////////////////////////////////////////////////////
//
void* Thread::threadRun(void *pParam)
{
	Thread *pThis = (Thread*) pParam;

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

	pThis->run();
	pThis->reportStopped();

	pthread_exit(NULL);
}

////////////////////////////////////////////////////////////////////////////////
//
bool Thread::start()
{
	mutex.lock();
	if (threadHandle == 0)
	{
		stopRequest = false;
		stopComplete = false;

		int rc = pthread_create(&threadHandle, NULL, threadRun, (void*) this);
		if (rc == 0)
		{
			mutex.unlock();
			return true;
		}
	}

	mutex.unlock();
	return false;
}

////////////////////////////////////////////////////////////////////////////////
//
bool Thread::stop(int timeOut)
{
	mutex.lock();
	if (threadHandle == 0)
	{
		mutex.unlock();
		return false;
	}

	stopRequest = true;		// raise thread stop flag

	for (int i = 0; (i < timeOut) && !stopComplete; i += 100)
	{
		mutex.unlock();
		sleep(100);
		mutex.lock();
	}

	stopRequest = false;
	if (!stopComplete)		// thread failed to shut down politely
	{
		if (pthread_cancel(threadHandle) != 0)
		{
			mutex.unlock();
			return false;
		}
	}

	pthread_join(threadHandle, NULL);
	threadHandle = 0;

	mutex.unlock();
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//
bool Thread::terminate()
{
	mutex.lock();
	if (threadHandle == 0)
	{
		mutex.unlock();
		return false;
	}

	if (pthread_cancel(threadHandle) != 0)
	{
		mutex.unlock();
		return false;
	}

	pthread_join(threadHandle, NULL);
	threadHandle = 0;

	mutex.unlock();
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//
bool Thread::stopRequested()
{
	mutex.lock();
	bool result = stopRequest;
	mutex.unlock();
	return result;
}

////////////////////////////////////////////////////////////////////////////////
//
void Thread::reportStopped()
{
	mutex.lock();
	stopRequest = false;
	stopComplete = true;
	mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
//
Semaphore::Semaphore()
{
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
}

Semaphore::~Semaphore()
{
	pthread_mutex_unlock(&mutex);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}

////////////////////////////////////////////////////////////////////////////////
//
void Semaphore::post()
{
	pthread_mutex_lock(&mutex);
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
}

////////////////////////////////////////////////////////////////////////////////
//
bool Semaphore::pend()
{
	pthread_mutex_lock(&mutex);

	bool ret = pthread_cond_wait(&cond, &mutex) == 0;
	pthread_mutex_unlock(&mutex);

	return ret;
}

////////////////////////////////////////////////////////////////////////////////
//
bool Semaphore::pend(int ms)
{
	pthread_mutex_lock(&mutex);

	timeval tp;
	gettimeofday(&tp, NULL);

	timespec ts;
	ts.tv_sec = tp.tv_sec + ms / 1000;
	long usec = tp.tv_usec + ((ms % 1000) * 1000);
	ts.tv_sec += usec / 1000000;
	ts.tv_nsec = (usec % 1000000) * 1000;

	bool ret = pthread_cond_timedwait(&cond, &mutex, &ts) == 0;
	pthread_mutex_unlock(&mutex);

	return ret;
}

////////////////////////////////////////////////////////////////////////////////
//
Mutex::Mutex()
{
	pthread_mutex_init(&mutex, NULL);
}

Mutex::~Mutex()
{
	pthread_mutex_unlock(&mutex);
	pthread_mutex_destroy(&mutex);
}

////////////////////////////////////////////////////////////////////////////////
//
bool Mutex::lock()
{
	return pthread_mutex_lock(&mutex) == 0;
}

////////////////////////////////////////////////////////////////////////////////
//
void Mutex::unlock()
{
	pthread_mutex_unlock(&mutex);
}
