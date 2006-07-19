
#include "toolkit.h"

void * Java_java_awt_Toolkit_fntInitFont (JNIEnv *env, jclass clazz, jstring jSpec, jint style, jint size)
{
	char *spec;
	int italic = 0;
	int weight = 0;
	s_font_t *font;
	
	DEBUGF("Enter");
	
	spec = java2CString(env, jSpec);
	if (style & 1) {
		weight = 1;
	}
	if (style & 2) {
		italic = 1;
	}
	DEBUGF("spec: %s, weight: %d, italic: %d, size: %d", spec, weight, italic, size);
	
	s_font_init(&font, "arial.ttf");
	s_font_set_size(font, size);

	AWT_FREE(spec);

	DEBUGF("Leave");

	return JCL_NewRawDataObject(env, font);
}

KAFFE_FONT_FUNC_DECL (jobject, Java_java_awt_Toolkit_fntInitFontMetrics)
{
	DEBUGF("Enter");
	DEBUGF("Leave");
	return jfont;
}

KAFFE_FONT_FUNC_DECL (jint, Java_java_awt_Toolkit_fntGetHeight)
{
	s_font_t *font = UNVEIL_FONT(jfont);
	DEBUGF("Enter");
	DEBUGF("Leave");
	return font->ascent - font->descent + 1;
}

KAFFE_FONT_FUNC_DECL (jint, Java_java_awt_Toolkit_fntGetDescent)
{
	DEBUGF("Enter");
	DEBUGF("Leave");
	return -(UNVEIL_FONT(jfont)->descent);
}

KAFFE_FONT_FUNC_DECL (jint, Java_java_awt_Toolkit_fntGetMaxDescent)
{
	DEBUGF("Enter");
	DEBUGF("Leave");
	return -(UNVEIL_FONT(jfont)->descent);
}

KAFFE_FONT_FUNC_DECL (jint, Java_java_awt_Toolkit_fntGetLeading)
{
	DEBUGF("Enter");
	DEBUGF("Leave");
	return 0;
}

KAFFE_FONT_FUNC_DECL (jint, Java_java_awt_Toolkit_fntGetAscent)
{
	DEBUGF("Enter");
	DEBUGF("Leave");
	return UNVEIL_FONT(jfont)->ascent;
}

KAFFE_FONT_FUNC_DECL (jint, Java_java_awt_Toolkit_fntGetMaxAscent)
{
	DEBUGF("Enter");
	DEBUGF("Leave");
	return UNVEIL_FONT(jfont)->ascent;
}

KAFFE_FONT_FUNC_DECL (jint, Java_java_awt_Toolkit_fntGetMaxAdvance)
{
	DEBUGF("Enter");
	DEBUGF("Leave");
	return UNVEIL_FONT(jfont)->height;
}

KAFFE_FONT_FUNC_DECL (jint, Java_java_awt_Toolkit_fntGetFixedWidth)
{
	DEBUGF("Enter");
	DEBUGF("Leave");
	return UNVEIL_FONT(jfont)->height;
}

KAFFE_FONT_FUNC_DECL (jint, Java_java_awt_Toolkit_fntIsWideFont)
{
	DEBUGF("Enter");
	DEBUGF("Leave");
	return 0;
}
