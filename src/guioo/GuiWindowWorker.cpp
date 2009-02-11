/***************************************************************************
 *   Copyright            : (c) 2007-2008 Artec Design LLC                 *
 *                        : (c) 2007-2008 Andrei Birjukov                  *
 *                        : (c) 2007-2008 Anti Sullin                      *
 *   Contact              : andrei.birjukov at artecdesign dot ee          *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include <stdio.h>
#include "GuiWindowWorker.h"

//#define DEBUG

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Class construcror
 *
 * Default constructor for the window worker objects
 */
GuiWindowWorker::GuiWindowWorker()
{
	jobThread = 0;
	jobData = NULL;
	jobIdentifier = -1;
	jobRunning = false;
	jobTerminate = false;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Clas destructor
 *
 * Destructor for the window worker objects
 * Joins the woker thread, to properly release resources.
 */
GuiWindowWorker::~GuiWindowWorker()
{
	if(jobThread != 0) pthread_join(jobThread, NULL);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Start a new job
 *
 * Launches a new worker thread with the given job identifier and parameters.
 *
 * Only one jon is currently supported, perhaps need to add support for multiple
 * parallel jobs in the future
 *
 * \param jobId Unique job identifier that is used later to stop the job and
 * is also passed to the callback functions.
 *
 * \param pData Pointer to the user data that is passed to job callback
 * functions.
 *
 * \return Returns true if the job was started successfully.
 */
bool GuiWindowWorker::startJob(int jobId, void* pData)
{
	if(jobThread != 0) pthread_join(jobThread, NULL);

	jobIdentifier = jobId;
	jobData = pData;
	jobTerminate = false;

	if(pthread_create(&jobThread, NULL, threadProc, (void*)this) != 0)
		return false;

	#ifdef DEBUG
	printf("Created worker thread\n");
	#endif

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Cancel a running job
 *
 * Terminate the currently running job. The worker thread may be cancelled at
 * any valid termination point. The worker thread is safely joined afterwards
 * to release resources. Note that cancellation is not safe in the sense that
 * it may leave some mutexes locked if a worker thread gets terminated at an
 * unstable point.
 *
 * \param jobId Identifier of the job
 *
 * \return Returns true if the job was cancelled successfully
 */
bool GuiWindowWorker::cancelJob(int jobId)
{
	if(jobThread == 0) return false;

	if(pthread_cancel(jobThread) != 0) return false;

	#ifdef DEBUG
	printf("Cancelled worker thread\n");
	#endif

	pthread_join(jobThread, NULL);
	jobThread = 0;

	jobFinished(jobIdentifier, jobData, false);
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Stop a running job
 *
 * Stops the currently running job in a polite way, by using a termination
 * flag (jobTerminate). The worker thread must poll this flag in the
 * jobStarted() function and return as soon as possible when jobTerminate
 * is set to false. The worker thread is joined afterwards to release its
 * resources.
 *
 * \param jobId Identifier of the job
 * \return Returns true if the thread is stopped successfully.
 */
bool GuiWindowWorker::stopJob(int jobId)
{
	if(jobThread == 0) return false;

	jobTerminate = true;

	pthread_join(jobThread, NULL);
	jobThread = 0;

	#ifdef DEBUG
	printf("Stopped worker thread\n");
	#endif

	return true;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Check if a job is running
 *
 * Returns the state of the specified job.
 *
 * \param idJob Identifier of the job being checked
 * \return Returns true if the specified job is currently running.
 */
bool GuiWindowWorker::jobInProgress(int idJob)
{
	return jobRunning;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Check if a job is terminating
 *
 * Returns the state of the specified job.
 *
 * \param idJob Identifier of the specified job.
 * \return Returns true if the specified job is currently terminating.
 */
bool GuiWindowWorker::jobTerminating(int idJob)
{
	return jobTerminate;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Worker thread routine
 *
 * Implements the worker thread. First, the jobStarted() virtual function
 * is called to start the main processing. After the job is complete on its
 * own, or stopped/cancelled externally the jobFinished() virtual function
 * is called to notify about termination of the job.
 *
 * \param pParam Pointer to the GuiWorkerThread object
 * \return NULL
 */
void* GuiWindowWorker::threadProc(void* pParam)
{
	GuiWindowWorker* pThis = (GuiWindowWorker*)pParam;
	pThis->jobRunning = true;

	pThis->jobStarted(pThis->jobIdentifier, pThis->jobData);
	pThis->jobFinished(pThis->jobIdentifier, pThis->jobData, !pThis->jobTerminate);

	pThis->jobRunning = false;
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Starting function of a job
 *
 * Given virtual function is called in the context of the worker thread when
 * the main processing must be initiated.
 *
 * Override this function to implement a window worker.
 *
 * \param jobId Identifier of the job that has been started
 * \param pData Pointer to the user data of the job
 */
void GuiWindowWorker::jobStarted(int jobId, void* pData)
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Finishing function of a job
 *
 * Given virtual function is called when the worker thread has either
 * finished its processing, or the job is cancelled or terminated externally.
 *
 * Override this function to implement a window worker.
 *
 * \param jobId Identifier of the job that has been started
 * \param pData Pointer to the user data of the job
 * \param successFlag Indicates if the job has been completed successfully.
 * Given flag is set to false if the job is either stopped or cancelled.
 */
void GuiWindowWorker::jobFinished(int jobId, void* pData, bool successFlag)
{
}
