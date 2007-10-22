
#include "config.h"

#if defined(CONFIG_VIDEO_HELPER_IRAIR)

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../../../lib/xynth_.h"
#include "../../server.h"
#include "helper.h"

static int irair_fd = -1;

int s_video_helper_irair_init (void)
{
	irair_fd = open("/dev/irr", O_RDONLY);
	return irair_fd;
}

int s_video_helper_irair_uninit (void)
{
	if (irair_fd >= 0) {
		close(irair_fd);
	}
	return 0;
}

char * s_video_helper_irair_getcode (void)
{
	unsigned int code;
	static char irair_code[255];
	if (read(irair_fd, &code, sizeof(code)) != sizeof(code)) {
		return NULL;
	}
	sprintf(irair_code, "0x%08x", code);
	return irair_code;
}

#endif
