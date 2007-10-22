
#if defined(VIDEO_HELPER_IRIMAF)

#include <stdio.h>
#include <stdlib.h>
#include <imaf_i2c.h>

#include "../../../lib/xynth_.h"
#include "../../server.h"
#include "helper.h"

int s_video_helper_irimaf_init (void)
{
	return imaf_i2c_open();
}

int s_video_helper_irimaf_uninit (void)
{
	imaf_i2c_close();
	return 0;
}

char * s_video_helper_irimaf_getcode (void)
{
	char ircode[5];
	static char irimaf_code[255];
	if (imaf_i2c_get_irr_data(ircode, 5) <= 0) {
		return NULL;
	}
	sprintf(irimaf_code, "0x%08x", ircode[0]);
	return irimaf_code;
}

#endif
