#ifndef GUIWINDOWWORKER_H_
#define GUIWINDOWWORKER_H_

#include <pthread.h>

#include "GuiWindow.h"

/**
 * \brief Window worker thead implementation
 * 
 * Given class implements a window worker thread by wrapping a set of pthead 
 * library functions. The intended usage of the window worker is inheritance
 * of the user window from both GuiWindow and GuiWindowWorker.
 * 
 * The user may the launch and manage a separate job via window worker 
 * interface. This may be required if some processing job needs to be performed
 * while the window itself must not be blocked and user interface events 
 * need to be processed in parallel.
 * 
 * The window worker notifies the caller about the progress of the job via 
 * virtual functions.
 * 
 * \author Andrei Birjukov
 */

class GuiWindowWorker
{
public:

	GuiWindowWorker();
	virtual ~GuiWindowWorker();

	bool startJob(int jobId, void* pData);
	bool stopJob(int jobId);
	bool cancelJob(int jobId);
	bool jobInProgress(int jobId);
	bool jobTerminating(int jobId);
	
protected:

	virtual void jobStarted(int jobId, void* pData);
	virtual void jobFinished(int jobId, void* pData, bool successFlag);
	
private:

	static void*	threadProc(void* pParam);

	pthread_t		jobThread;
	void*			jobData;
	int				jobIdentifier;
	bool			jobRunning;
	bool			jobTerminate;
};

#endif /*GUIWINDOWWORKER_H_*/
