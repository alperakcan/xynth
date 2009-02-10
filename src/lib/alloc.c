/***************************************************************************
    begin                : Tue Oct 4 2005
    copyright            : (C) 2005 - 2008 by Alper Akcan
    email                : alper.akcan@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include <stdlib.h>
#include <pthread.h>
#include <execinfo.h>
#include "xynth_.h"

#if defined(CONFIG_LEAK_DETECT)
static int trace_enable = 0;
static pthread_mutex_t trace_mutex;

typedef struct alloc_record_s {
	void *ptr;
	unsigned int size;
	void *callstack[CONFIG_LEAK_DEPTH];
	struct alloc_record_s *next;
	struct alloc_record_s *prev;
} alloc_record_t;

alloc_record_t *head = NULL;
alloc_record_t *tail = NULL;
#endif

void * s_malloc (unsigned int size)
{
	void *ret;
	ret = malloc(size);
	if (ret == NULL) {
		debugf(DFAT, "Not enough memory!");
	}
#if defined(CONFIG_LEAK_DETECT)
	if (trace_enable) {
		pthread_mutex_lock(&trace_mutex);
		if(head == NULL) {
			head = malloc(sizeof(alloc_record_t));
			memset(head, 0, sizeof(alloc_record_t));
			tail = head;
		} else {
			alloc_record_t *rec = malloc(sizeof(alloc_record_t));
			memset(rec, 0, sizeof(alloc_record_t));
			tail->next = rec;
			rec->prev = tail;
			tail = rec;
		}
		tail->ptr = ret;
		tail->size = size;
		backtrace(tail->callstack, CONFIG_LEAK_DEPTH);
		pthread_mutex_unlock(&trace_mutex);
	}
#endif
	return ret;
}

void * s_calloc (unsigned int nmemb, unsigned int size)
{
	void *ret;
	ret = s_malloc(nmemb * size);
	if (ret == NULL) {
		debugf(DFAT, "Not enough memory!");
	}
	memset(ret, 0, nmemb * size);
	return ret;
}

void * s_realloc (void *ptr, unsigned int size)
{
	void *ret;
	ret = realloc(ptr, size);
	if (ret == NULL) {
		debugf(DFAT, "Not enough memory!");
	}
#if defined(CONFIG_LEAK_DETECT)
	if (trace_enable) {
		pthread_mutex_lock(&trace_mutex);
		alloc_record_t *rec = tail;
		int found = 0;
		while (rec != NULL) {
			if (rec->ptr == ptr) {
				rec->ptr = ret;
				rec->size = size;
				memset(rec->callstack, 0, sizeof(rec->callstack));
				backtrace(tail->callstack, CONFIG_LEAK_DEPTH);
				found = 1;
				break;
			}
			rec = rec->prev;
		}
#if defined(CONFIG_LEAK_NONEXISTING)
		if (found == 0) {
			fprintf(stderr, "realloc() on non-existing block %p\n", ptr);
			if (head == NULL) {
				head = malloc(sizeof(alloc_record_t));
				memset(head, 0, sizeof(alloc_record_t));
				tail = head;
			} else {
				alloc_record_t *rec = malloc(sizeof(alloc_record_t));
				memset(rec, 0, sizeof(alloc_record_t));
				tail->next = rec;
				rec->prev = tail;
				tail = rec;
			}
			tail->ptr = ret;
			tail->size = size;
			backtrace(tail->callstack, CONFIG_LEAK_DEPTH);
		}
#endif
		pthread_mutex_unlock(&trace_mutex);
	}
#endif
	return ret;
}

void s_free (void *ptr)
{
	if (ptr != NULL) {
		free(ptr);
#if defined(CONFIG_LEAK_DETECT)
		if (trace_enable) {
			pthread_mutex_lock(&trace_mutex);
			alloc_record_t *rec = tail;
			int found = 0;
			while (rec != NULL) {
				if (rec->ptr == ptr) {
					if (rec->prev != NULL) {
						rec->prev->next = rec->next;
					}
					if (rec->next != NULL) {
						rec->next->prev = rec->prev;
					}
					if (rec == tail) {
						tail = rec->prev;
					}
					if (rec == head) {
						head = rec->next;
					}
					free(rec);
					found = 1;
					break;
				}
				rec = rec->prev;
			}
#if defined(CONFIG_LEAK_NONEXISTING)
			if (found == 0) {
				int i;
				void *callstack[CONFIG_LEAK_DEPTH]={0};
				backtrace(callstack, CONFIG_LEAK_DEPTH);
				fprintf(stderr, "free() on non-existing block %p at:", ptr);
				for(i = 0; i < CONFIG_LEAK_DEPTH; i ++) {
					fprintf(stderr, "%p ", callstack[i]);
				}
				fprintf(stderr, "\n");
			}
#endif
			pthread_mutex_unlock(&trace_mutex);
		}
#endif
	}
}

void s_trace_start (void)
{
#if defined(CONFIG_LEAK_DETECT)
	pthread_mutex_init(&trace_mutex, NULL);
	trace_enable = 1;
#endif
}

void s_trace_stop (void)
{
#if defined(CONFIG_LEAK_DETECT)
	alloc_record_t *rec = head;
	int i;
	int nbytes = 0;
	int nblocks = 0;

	pthread_mutex_lock(&trace_mutex);
	trace_enable = 0;
	pthread_mutex_destroy(&trace_mutex);

	while (rec != NULL) {
		fprintf(stderr, "Leaked block: %p, size %d, backtrace: ", rec->ptr, rec->size);
		for(i = 0; i < CONFIG_LEAK_DEPTH; i ++) {
			printf("%p ", rec->callstack[i]);
		}
		fprintf(stderr, "\n");
		nblocks++;
		nbytes += rec->size;
		rec = rec->next;
	}
	fprintf(stderr, "Total %d bytes in %d blocks\n", nbytes, nblocks);
#endif
}
