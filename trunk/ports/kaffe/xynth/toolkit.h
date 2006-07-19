
#include "config.h"
#include "config-std.h"
#include "config-mem.h"
#include "config-setjmp.h"

#include "jcl.h"

#include <xynth.h>

typedef struct xynth_toolkit_s {
	s_window_t *root;
} xynth_toolkit_t;

xynth_toolkit_t *xynth;

#define DEBUGF(a...)	printf(a); printf(" (%s [%s:%d])\n", __FUNCTION__, __FILE__, __LINE__);
#define NYI()           DEBUGF("Not Yet Implemented !");

#define TLK_IS_BLOCKING			1
#define TLK_IS_DISPATCH_EXCLUSIVE	2
#define TLK_NEEDS_FLUSH			4
#define TLK_NATIVE_DISPATCHER_LOOP	8
#define TLK_EXTERNAL_DECO		16

static inline void * _awt_malloc_wrapper (size_t size)
{
	void *adr;
	enterUnsafeRegion();
	adr = malloc(size);
	leaveUnsafeRegion();
	return adr;
}

static inline void * _awt_calloc_wrapper (size_t n, size_t size)
{
	void *adr;
	enterUnsafeRegion();
	adr = calloc(n, size);
	leaveUnsafeRegion();
	return adr;
}

static inline void _awt_free_wrapper (void *adr)
{
	enterUnsafeRegion();
	free(adr);
	leaveUnsafeRegion();
}

#define AWT_MALLOC(_n)		_awt_malloc_wrapper(_n)
#define AWT_CALLOC(_n,_sz)	_awt_calloc_wrapper(_n, _sz)
#define AWT_FREE(_adr)		_awt_free_wrapper(_adr);

static inline char * java2CString (JNIEnv *env, jstring jstr)
{
	char *buf;
	jboolean isCopy;
	register unsigned int i;
	jsize n = (*env)->GetStringLength(env, jstr);
	const jchar *jc = (*env)->GetStringChars(env, jstr, &isCopy);
	buf = AWT_MALLOC(n + 1);
	for (i = 0; i < n; i++) {
		buf[i] = (char) jc[i];
	}
	buf[i] = 0;
	(*env)->ReleaseStringChars(env, jstr, jc);
	return buf;
}

#define KAFFE_FONT_FUNC_DECL( ret, name, args... ) ret name(JNIEnv* env, jclass clazz, jobject jfont, ##args)
#define UNVEIL_FONT(jfont) ((s_font_t *) JCL_GetRawData(env, jfont))
