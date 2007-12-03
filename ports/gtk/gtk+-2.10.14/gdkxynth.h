
#include <config.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "gdk.h"
#include "gdkwindow.h"

#include "gdkregion-generic.h"

#include "gdkinternals.h"
#include "gdkalias.h"

#define XYNTH_DEBUG 99

#if XYNTH_DEBUG != 0
#define DEBUG(fmt...) {\
	printf(fmt);\
	printf(" [%s (%s:%d)]\n", __FUNCTION__, __FILE__, __LINE__);\
}
#else
#define DEBUG(fmt...) do { } while (0)
#endif

#if XYNTH_DEBUG == 99
#define ENTER() DEBUG("Enter");
#define LEAVE() DEBUG("Leave");
#else
#endif

#define NIY()   {\
	DEBUG("Not Implemented Yet");\
}

#define ASSERT() {\
	DEBUG("This should not happen");\
	assert(0);\
}

#define ERROR(fmt...) {\
	DEBUG("ERROR" fmt); \
	ASSERT(); \
}
