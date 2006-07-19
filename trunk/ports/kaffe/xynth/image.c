
#include "toolkit.h"


jobject Java_java_awt_Toolkit_imgCreateFromFile (JNIEnv* env, jclass clazz, jstring fileName)
{
	char *fname;
	s_image_t *img;
	
	fname = java2CString(env, fileName);
	s_image_init(&img);
	if (s_image_img(fname, img)) {
		s_image_uninit(img);
		AWT_FREE(gname);
		return NULL;
	}
	AWT_FREE(fname);
	return JCL_NewRawDataObject(env, img);
}
