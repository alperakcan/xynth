
#include "config.h"
#include "config-std.h"
#include "config-mem.h"
#include "config-setjmp.h"

#include "jcl.h"

#include <xynth.h>

#define XYNTH_NWINDOWS 100

typedef struct graphics_s {
	s_surface_t *surface;
	int fg;
	int bg;
	int x0;
	int y0;
} graphics_t;

typedef struct window_rec_s {
	s_window_t *window;
	s_window_t *owner;
} window_rec_t;

typedef struct deco_inset_s {
	int left;
	int top;
	int right;
	int bottom;
	char guess;
} deco_inset_t;

typedef struct xynth_toolkit_s {
	deco_inset_t frame_insets;
	unsigned int nwindows;
	window_rec_t *windows;
	window_rec_t *last_window;
	unsigned int last_idx;
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

#define KAFFE_FONT_FUNC_DECL(ret, name, args...) ret name(JNIEnv *env, jclass clazz, jobject jfont, ##args)
#define UNVEIL_FONT(jfont) ((s_font_t *) JCL_GetRawData(env, jfont))

#define KAFFE_IMG_FUNC_DECL(ret, name, args...) ret name(JNIEnv *env, jclass clazz, jobject jimg, ##args)
#define UNVEIL_IMG(jimg) ((s_image_t *) JCL_GetRawData(env, jimg))

#define UNVEIL_WINDOW(jwin) ((s_window_t *) JCL_GetRawData(env, jwin))

#define UNVEIL_GRAP(jgraphics) ((graphics_t *) JCL_GetRawData(env, jgraphics));

static inline int source_idx_free (xynth_toolkit_t *tk, s_window_t *win)
{
	register int n;
	for (n = 0; n < tk->nwindows; n++) {
		if (tk->windows[n].window <= 0 ) {
			tk->last_idx = n;
			tk->last_window = win;
			return n;
		}
	}
	return -1;
}

static inline int source_idx_register (xynth_toolkit_t *tk, s_window_t *window, s_window_t *owner)
{
	int i;
	i = source_idx_free(tk, window);
	if (i >= 0) {
		tk->windows[i].window = window;
		tk->windows[i].owner = owner;
		return i;
	} else {
		DEBUGF("window table out of spafe: %d", tk->nwindows);
		return -1;
	}
}

static inline int source_idx_get (xynth_toolkit_t *tk, s_window_t *win)
{
	register int n;
	if (win == tk->last_window) {
		return tk->last_idx;
	} else {
		for (n = 0; n < tk->nwindows; n++) {
			if (tk->windows[n].window == win) {
				tk->last_idx = n;
				tk->last_window = win;
				return tk->last_idx;
			} else if (tk->windows[n].window == 0) {
				return -1;
			}
		}
		return -1;
	}
}
