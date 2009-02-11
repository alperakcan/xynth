
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/ioctl.h>

#include "../../../lib/xynth_.h"
#include "server.h"
#include "../helper/helper.h"
#include "fbdev.h"

#ifndef KEY_KBDILLUMTOGGLE
#define KEY_KBDILLUMTOGGLE		228
#endif

static int fdKeypad = -1;

int armfb_keypad_init (s_server_conf_t *cfg)
{
	char devFile[32];
	unsigned char bitMask[EV_MAX/8 + 1];
	int i;

	debugf(DSER, "ARTEC : keypad driver init");

	// cycle through event interfaces to discover a keypad
	for(i = 0; i < 32; ++i)
	{
		sprintf(devFile, "/dev/input/event%d", i);
		fdKeypad = open(devFile, O_RDWR);

		if(fdKeypad >= 0)
		{
			if(ioctl(fdKeypad, EVIOCGBIT(0, sizeof(bitMask)), bitMask) < 0)
			{
				close(fdKeypad);
				fdKeypad = -1;
			}

			// detect a keypad driver by presense of the KEY bit
			if(bitMask[EV_KEY/8] & (1<<(EV_KEY % 8)))
			{
				debugf(DSER, "ARTEC: using keypad interface %s", devFile);
				return fdKeypad;
			}
			else
			{
				debugf(DSER, "ARTEC: interface %s is not a keypad", devFile);
				close(fdKeypad);
				fdKeypad = -1;
			}
		}
	}

	debugf(DSER | DSYS | DFAT, "ARTEC : failed opening keypad device");
	return -1;
}

void armfb_keypad_uninit (void)
{
	debugf(DSER, "ARTEC : keypad driver deinit");

	if(fdKeypad >= 0 )
	{
		close(fdKeypad);
		fdKeypad = -1;
	}
}

int armfb_keypad_update (s_video_input_data_t *keybd)
{
	struct input_event eventData;
	int readBytes;

	while(1)
	{
		readBytes = read(fdKeypad, &eventData, sizeof(eventData));
		if(readBytes != sizeof(eventData)) return -1;

		if(eventData.type == EV_KEY)
		{
			debugf(DSER, "ARTEC : keypad %d -> %d", eventData.code, eventData.value);
			keybd->keybd.state = eventData.value ? EVENT_TYPE_KEYBOARD_PRESSED : EVENT_TYPE_KEYBOARD_RELEASED;

			switch(eventData.code)
			{
			case KEY_0:
				keybd->keybd.keycode = KEYBOARD_BUTTON_ZERO;
				break;
			case KEY_1:
				keybd->keybd.keycode = KEYBOARD_BUTTON_ONE;
				break;
			case KEY_2:
				keybd->keybd.keycode = KEYBOARD_BUTTON_TWO;
				break;
			case KEY_3:
				keybd->keybd.keycode = KEYBOARD_BUTTON_THREE;
				break;
			case KEY_4:
				keybd->keybd.keycode = KEYBOARD_BUTTON_FOUR;
				break;
			case KEY_5:
				keybd->keybd.keycode = KEYBOARD_BUTTON_FIVE;
				break;
			case KEY_6:
				keybd->keybd.keycode = KEYBOARD_BUTTON_SIX;
				break;
			case KEY_7:
				keybd->keybd.keycode = KEYBOARD_BUTTON_SEVEN;
				break;
			case KEY_8:
				keybd->keybd.keycode = KEYBOARD_BUTTON_EIGHT;
				break;
			case KEY_9:
				keybd->keybd.keycode = KEYBOARD_BUTTON_NINE;
				break;
			case KEY_BACKSPACE:
				keybd->keybd.keycode = KEYBOARD_BUTTON_DELETE;
				break;
			case KEY_ENTER:
				keybd->keybd.keycode = KEYBOARD_BUTTON_RETURN;
				break;
			case KEY_MINUS:
				keybd->keybd.keycode = KEYBOARD_BUTTON_MINUS;
				break;
			case KEY_DOT:
				keybd->keybd.keycode = KEYBOARD_BUTTON_PERIOD;
				break;
			case KEY_UP:
				keybd->keybd.keycode = KEYBOARD_BUTTON_UP;
				break;
			case KEY_DOWN:
				keybd->keybd.keycode = KEYBOARD_BUTTON_DOWN;
				break;
			case KEY_POWER:
				keybd->keybd.keycode = KEYBOARD_BUTTON_ESCAPE;
				break;
			case KEY_F1:
				keybd->keybd.keycode = KEYBOARD_BUTTON_F1;
				break;
			case KEY_F2:
				keybd->keybd.keycode = KEYBOARD_BUTTON_F2;
				break;
			case KEY_F3:
				keybd->keybd.keycode = KEYBOARD_BUTTON_F3;
				break;
			case KEY_F4:
				keybd->keybd.keycode = KEYBOARD_BUTTON_F4;
				break;
			case KEY_F5:
				keybd->keybd.keycode = KEYBOARD_BUTTON_F5;
				break;
			case KEY_F11:
				keybd->keybd.keycode = KEYBOARD_BUTTON_F11;   // "shift"
				break;
			case KEY_MENU:
				keybd->keybd.keycode = KEYBOARD_BUTTON_F12;   // "menu"
				break;
			case KEY_F13:
				keybd->keybd.keycode = KEYBOARD_BUTTON_F13;   // unused by hardware
				break;
			case KEY_F14:
				keybd->keybd.keycode = KEYBOARD_BUTTON_F14;   // unused by hardware
				break;

			default:
				keybd->keybd.keycode = 0;
				break;
			}

			keybd->keybd.scancode = eventData.code;
			keybd->keybd.button = 0;
			keybd->keybd.ascii = 0;
		}
		else if(eventData.type == EV_SYN) break;
	}

	return 0;
}
