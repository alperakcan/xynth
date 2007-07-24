#ifndef GDKINPUTXYNTH_H_
#define GDKINPUTXYNTH_H_

extern int _gdk_xynth_mouse_x;
extern int _gdk_xynth_mouse_y;
extern GdkModifierType _gdk_xynth_modifiers;

struct _GdkDeviceClass {
	GObjectClass parent_class;
};

#endif /*GDKINPUTXYNTH_H_*/
