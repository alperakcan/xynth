#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>

class Mutex
{
public:

	Mutex();
	virtual ~Mutex();

	bool lock();
	void unlock();

private:

	pthread_mutex_t mutex;
};

class Thread
{
public:
	Thread();
	virtual ~Thread();

	static bool sleep(int msec);

	bool start();
	bool stop(int timeOut = 10000);
	bool terminate();

protected:

	virtual void run();
	static void* threadRun(void *pParam);

	bool stopRequested();
	void reportStopped();

	pthread_t threadHandle;

private:

	bool stopRequest;
	bool stopComplete;
	Mutex mutex;
};

class Semaphore
{
public:

	Semaphore();
	virtual ~Semaphore();

	void post();
	bool pend();
	bool pend(int ms);

private:

	pthread_mutex_t mutex;
	pthread_cond_t cond;
};

#endif /*THREAD_H_*/
