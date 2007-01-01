/***************************************************************************
    begin                : Fri Nov 24 2006
    copyright            : (C) 2006 - 2007 by Alper Akcan
    email                : distchx@yahoo.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include "xynth_.h"

typedef struct s_gettext_header_s {
	char magic[8];
	unsigned long int count;
} s_gettext_header_t;

typedef struct s_gettext_xo_s {
	unsigned long int hash;
	unsigned long int id_len;
	unsigned long int str_len;
	unsigned long int id_offset;
	unsigned long int str_offset;
} s_gettext_xo_t;

typedef struct s_gettext_msg_s {
	unsigned int pos;
	unsigned long int hash;
} s_gettext_msg_t;

struct s_gettext_s {
	FILE *file;
	char *buf;
	char *dir;
	char *domain;
	char *locale;
	int category;
	unsigned int count;
	s_gettext_xo_t *msgs;
	unsigned int ucount;
	s_gettext_msg_t *umsgs;
	s_thread_mutex_t *mutex;
};

static unsigned long int hash_string (const char *str_param)
{
	#define HASHWORDBITS 32
	unsigned long int hval, g;
	const char *str = str_param;
	/* Compute the hash value for the given string.  */
	hval = 0;
	while (*str != '\0') {
		hval <<= 4;
		hval += (unsigned char) *str++;
		g = hval & ((unsigned long int) 0xf << (HASHWORDBITS - 4));
		if (g != 0) {
			hval ^= g >> (HASHWORDBITS - 8);
			hval ^= g;
		}
	}
	return hval;
}

static void w_changelocale_ (s_window_t *window)
{
	unsigned int i;
	unsigned int u;
	unsigned long int h;
	for (i = 0, u = 0, h = 0; i < window->gettext->count; i++) {
		if (h != window->gettext->msgs[i].hash) {
			h = window->gettext->msgs[i].hash;
			u++;
		}
	}
	window->gettext->ucount = u;
	window->gettext->umsgs = (s_gettext_msg_t *) malloc(sizeof(s_gettext_msg_t) * u);
	memset(window->gettext->umsgs, 0, sizeof(s_gettext_msg_t));
	for (i = 0, u = 0, h = 0; i < window->gettext->count; i++) {
		if (h != window->gettext->msgs[i].hash) {
			h = window->gettext->msgs[i].hash;
			window->gettext->umsgs[u].pos = i;
			window->gettext->umsgs[u].hash = h;
			u++;
		}
	}
}

static void w_changelocale (s_window_t *window)
{
	s_gettext_header_t lheader;
	if (window->gettext->domain &&
	    window->gettext->dir) {
	    	s_free(window->gettext->buf);
	    	window->gettext->buf = (char *) s_malloc(sizeof(char) * (strlen(window->gettext->dir) + strlen(window->gettext->locale) + strlen(window->gettext->domain) + 40));
	    	sprintf(window->gettext->buf, "%s/%s/LC_MESSAGES/%s.xo", window->gettext->dir, window->gettext->locale, window->gettext->domain);
	    	if (window->gettext->file) {
	    		fclose(window->gettext->file);
	    	}
	    	window->gettext->file = fopen(window->gettext->buf, "r");
	    	if (window->gettext->file == NULL) {
	    		return;
	    	}
		fseek(window->gettext->file, 0, SEEK_SET);
		fread(&lheader, sizeof(s_gettext_header_t), 1, window->gettext->file);
		window->gettext->count = lheader.count;
		s_free(window->gettext->msgs);
		s_free(window->gettext->umsgs);
		window->gettext->msgs = (s_gettext_xo_t *) s_malloc(sizeof(s_gettext_xo_t) * window->gettext->count);
		fread(window->gettext->msgs, sizeof(s_gettext_xo_t), window->gettext->count, window->gettext->file);
		w_changelocale_(window);
	}
}

char * s_setlocale(s_window_t *window, int category, const char *locale)
{
	s_thread_mutex_lock(window->gettext->mutex);
	window->gettext->category = category;
	if (locale) {
		s_free(window->gettext->locale);
		window->gettext->locale = strdup(locale);
	}
	w_changelocale(window);
	s_thread_mutex_unlock(window->gettext->mutex);
	return window->gettext->locale;
}

char * s_bindtextdomain (s_window_t *window, const char *domainname, const char *dirname)
{
	s_thread_mutex_lock(window->gettext->mutex);
	if (domainname) {
		s_free(window->gettext->domain);
		window->gettext->domain = strdup(domainname);
	}
	if (dirname) {
		s_free(window->gettext->dir);
		window->gettext->dir = strdup(dirname);
	}
	w_changelocale(window);
	s_thread_mutex_unlock(window->gettext->mutex);
	return window->gettext->dir;
}

char * s_textdomain (s_window_t *window, const char *domainname)
{
	s_thread_mutex_lock(window->gettext->mutex);
	if (domainname) {
		s_free(window->gettext->domain);
		window->gettext->domain = strdup(domainname);
	}
	w_changelocale(window);
	s_thread_mutex_unlock(window->gettext->mutex);
	return window->gettext->domain;
}

static inline int s_gettext_cmp (char *str, char *ptr)
{
	while (*str && *ptr) {
		if (*str++ != *ptr++) {
			return -1;
		}
	}
	if (*str || *ptr) {
		return -1;
	}
	return 0;
}

static inline int s_gettext_binsearch (s_gettext_msg_t *umsgs, unsigned long int hash, int low, int high)
{
	unsigned long int mid;
	while (low <= high) {
		mid = (low + high) / 2;
		if (hash > umsgs[mid].hash) {
			low = mid + 1;
		} else if (hash < umsgs[mid].hash) {
			high = mid - 1;
		} else {
			return mid;
		}
	}
	return ~0;
}

char * s_gettext (s_window_t *window, const char *str)
{
	unsigned long int i;
	unsigned long int h;
	s_thread_mutex_lock(window->gettext->mutex);
	if (str == NULL ||
	    window->gettext->file == NULL) {
	    	goto end;
	}
	h = hash_string(str);
	i = s_gettext_binsearch(window->gettext->umsgs, h, 0, window->gettext->ucount - 1);
	for (i = i; i < window->gettext->count; i++) {
		if (h == window->gettext->msgs[i].hash) {
			fseek(window->gettext->file, window->gettext->msgs[i].id_offset, SEEK_SET);
			s_free(window->gettext->buf);
			window->gettext->buf = (char *) malloc(sizeof(char) * (window->gettext->msgs[i].id_len + 1));
			fread(window->gettext->buf, sizeof(char), window->gettext->msgs[i].id_len + 1, window->gettext->file);
			if (s_gettext_cmp((char *) str, window->gettext->buf) == 0) {
				fseek(window->gettext->file, window->gettext->msgs[i].str_offset, SEEK_SET);
				s_free(window->gettext->buf);
				window->gettext->buf = (char *) s_malloc(sizeof(char) * (window->gettext->msgs[i].str_len + 1));
				fread(window->gettext->buf, sizeof(char), window->gettext->msgs[i].str_len + 1, window->gettext->file);
				str = window->gettext->buf;
				break;
			} else {
				s_free(window->gettext->buf);
				window->gettext->buf = NULL;
			}
		} else {
			s_free(window->gettext->buf);
			window->gettext->buf = NULL;
			break;
		}
	}
end:	s_thread_mutex_unlock(window->gettext->mutex);
	return (char *) str;
}

void s_gettext_uninit (s_window_t *window)
{
	s_thread_mutex_lock(window->gettext->mutex);
	s_free(window->gettext->buf);
	s_free(window->gettext->dir);
	s_free(window->gettext->domain);
	s_free(window->gettext->locale);
	s_free(window->gettext->msgs);
	s_free(window->gettext->umsgs);
	if (window->gettext->file) fclose(window->gettext->file);
	s_thread_mutex_unlock(window->gettext->mutex);
	s_thread_mutex_destroy(window->gettext->mutex);
	s_free(window->gettext);
}

int s_gettext_init (s_window_t *window)
{
	window->gettext = (s_gettext_t *) s_malloc(sizeof(s_gettext_t));
	window->gettext->file = NULL;
	window->gettext->buf = NULL;
	window->gettext->dir = NULL;
	window->gettext->domain = NULL;
	window->gettext->locale = NULL;
	window->gettext->category = 0;
	window->gettext->count = 0;
	window->gettext->msgs = NULL;
	window->gettext->ucount = 0;
	window->gettext->umsgs = NULL;
	s_thread_mutex_init(&(window->gettext->mutex));
	return 0;
}
