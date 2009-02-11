
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/ioctl.h>

#include "../../../lib/xynth_.h"
#include "server.h"
#include "../helper/helper.h"
#include "fbdev.h"

static int fdTouch = -1;
static int minX=0, maxX=0, minY=0, maxY=0;
static int touchX=0, touchY=0, touchPressure=0, touchButton=0;

int armfb_touch_init (s_server_conf_t *cfg)
{
	char devFile[32];
	unsigned char bitMask[EV_MAX/8 + 1];
	struct input_absinfo absInfo;
	int i;

	debugf(DSER, "ARTEC: touchscreen driver init");

	// cycle through event interfaces to discover touch screen
	for(i = 0; i < 32; ++i)
	{
		sprintf(devFile, "/dev/input/event%d", i);
		fdTouch = open(devFile, O_RDWR);

		if(fdTouch >= 0)
		{
			if(ioctl(fdTouch, EVIOCGBIT(0, sizeof(bitMask)), bitMask) < 0)
			{
				close(fdTouch);
				fdTouch = -1;
			}
			
			// detect a touchscreen driver by presense of the KEY bit
			if(bitMask[EV_ABS/8] & (1<<(EV_ABS % 8))) 
			{
				debugf(DSER, "ARTEC: using touchscreen interface %s", devFile);
				
				ioctl(fdTouch, EVIOCGABS(ABS_X), &absInfo);
				minX = absInfo.minimum;
				maxX = absInfo.maximum;

				ioctl(fdTouch, EVIOCGABS(ABS_Y), &absInfo);
				minY = absInfo.minimum;
				maxY = absInfo.maximum;
				
				return fdTouch;
			}
			else
			{
				debugf(DSER, "ARTEC: interface %s is not a touchscreen", devFile);
				close(fdTouch);
				fdTouch = -1;
			}
		}
	}

	debugf(DSER | DSYS | DFAT, "ARTEC: failed opening touchscreen device");
	return -1;
}

void armfb_touch_uninit (void)
{
	debugf(DSER | DSYS, "ARTEC: touchscreen driver deinit");

	if(fdTouch >= 0 )
	{
		close(fdTouch);
		fdTouch = -1;
	}
}

int armfb_touch_update (s_video_input_data_t *mouse)
{
	struct input_event eventData;
	int readBytes;
	
	while(1)
	{
		readBytes = read(fdTouch, &eventData, sizeof(eventData));
		if(readBytes != sizeof(eventData)) return 0;

		if(eventData.type == EV_ABS) switch(eventData.code)
		{
		case ABS_X:
			
			touchX = eventData.value;
			break;
			
		case ABS_Y:
			
			touchY = eventData.value;
			break;
			
		case ABS_PRESSURE:
			
			touchPressure = eventData.value;
			break;
		}
		else if(eventData.type == EV_KEY) switch(eventData.code)
		{
		case BTN_TOUCH:
			
			touchButton = eventData.value;
			break;
		}
		else if(eventData.type == EV_SYN) break;
	}

	mouse->mouse.x = ((touchX - minX) * armfb.v_scr.xres) / (maxX - minX);
	mouse->mouse.y = armfb.v_scr.yres - ((touchY - minY) * armfb.v_scr.yres) / (maxY - minY);
	mouse->mouse.buttons = touchButton ? MOUSE_BUTTON_LEFT : 0;
	
	debugf(DSER, "ARTEC: touch @ %d x %d [%d] - %d", 
			touchX, touchY, touchPressure, touchButton);
	return 0;
}
