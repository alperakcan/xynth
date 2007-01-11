
#include <config.h>
#include "gdk.h"
#include "gdkprivate.h"
#include "gdkalias.h"

#define NIY() {\
	static int __niy__ = 1;\
	if (__niy__) {\
		printf("NIY: %s (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);\
		__niy__ = 0;\
	}\
}

#define ENT() \
	static int __ent__ = 0;\
	printf("ENT [%d]: %s (%s:%d)\n", __ent__, __FUNCTION__, __FILE__, __LINE__);\
	__ent__++;

#define LEV() \
	printf("LEV [%d]: %s (%s:%d)\n", __ent__, __FUNCTION__, __FILE__, __LINE__);\
	__ent__--;

GdkAtom _gdk_selection_property;

GdkDisplay *_gdk_display;
