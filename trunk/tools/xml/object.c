
#include <xynth.h>
#include <widget.h>

void red_draw (w_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 255, 0, 0));
}

void green_draw (w_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 0, 255, 0));
}

void blue_draw (w_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 0, 0, 255));
}

void white_draw (w_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 255, 255, 255));
}
