
#include <assert.h>

#if 1
#define DEBUG(fmt...) {\
	printf(fmt);\
	printf(" [%s (%s:%d)]\n", __FUNCTION__, __FILE__, __LINE__);\
}
#else
#define DEBUG(fmt...) do { } while (0)
#endif

#define ENTER() DEBUG("Enter");
#define LEAVE() DEBUG("Leave");
#define NIY()   {\
	DEBUG("Not Implemented Yet");\
}
#define ASSERT() {\
	DEBUG("This should not happen");\
	assert(0);\
}
