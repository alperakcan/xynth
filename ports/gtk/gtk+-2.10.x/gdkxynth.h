
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define XYNTH_DEBUG_LEVEL 1

#if XYNTH_DEBUG_LEVEL > 0
#define DEBUG(fmt...) {\
	printf(fmt);\
	printf(" [%s (%s:%d)]\n", __FUNCTION__, __FILE__, __LINE__);\
}
#else
#define DEBUG(fmt...) do { } while (0)
#endif

#if XYNTH_DEBUG_LEVEL > 1
#define ENTER() DEBUG("Enter");
#define LEAVE() DEBUG("Leave");
#else
#define ENTER() do { } while (0)
#define LEAVE() do { } while (0)
#endif

#define NIY() {\
	DEBUG("Not Implemented Yet");\
}
#define ASSERT() {\
	DEBUG("This should not happen");\
	assert(0);\
}
