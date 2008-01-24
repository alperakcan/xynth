
#include "widgetx_xml.h"

static int frame_two_area_hide = 0;

void frame_two_area_red_draw (w_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 255, 0, 0));
}

void frame_two_area_green_draw (w_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 0, 255, 0));
}

void frame_two_area_blue_draw (w_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 0, 0, 255));
}

void frame_two_area_white_draw (w_object_t *object)
{
	s_fillbox(object->surface, 0, 0, object->surface->width, object->surface->height, s_rgbcolor(object->surface, 255, 255, 255));
}

void frame_two_button_hide_draw (w_object_t *object)
{
	w_button_draw(object);
	if (frame_two_button_hide_textbox == NULL) {
		return;
	}
	if (frame_two_area_hide) {
		w_textbox_set_str(frame_two_button_hide_textbox->object, "show area");
	} else {
		w_textbox_set_str(frame_two_button_hide_textbox->object, "hide area");
	}
}

void frame_two_button_hide_pressed (w_object_t *object, int button)
{
	frame_two_area_hide = (frame_two_area_hide + 1) % 2;
	if (frame_two_area_hide) {
		w_object_hide(frame_two_area->object);
		w_object_hide(frame_two_button_blender->object);
		w_object_hide(frame_two_button_focus->object);
	} else {
		w_object_show(frame_two_area->object);
		w_object_show(frame_two_button_blender->object);
		w_object_show(frame_two_button_focus->object);
	}
}

void frame_two_button_blender_pressed (w_object_t *object, int button)
{
	w_object_move(frame_two_area_red->object, (rand() + 1) % frame_two_area->object->surface->buf->w,
			                          (rand() + 1) % frame_two_area->object->surface->buf->h,
			                          (rand() + 1) % frame_two_area->object->surface->buf->w,
			                          (rand() + 1) % frame_two_area->object->surface->buf->h);
	w_object_move(frame_two_area_green->object, (rand() + 1) % frame_two_area->object->surface->buf->w,
			                            (rand() + 1) % frame_two_area->object->surface->buf->h,
			                            (rand() + 1) % frame_two_area->object->surface->buf->w,
			                            (rand() + 1) % frame_two_area->object->surface->buf->h);
	w_object_move(frame_two_area_blue->object, (rand() + 1) % frame_two_area->object->surface->buf->w,
			                           (rand() + 1) % frame_two_area->object->surface->buf->h,
			                           (rand() + 1) % frame_two_area->object->surface->buf->w,
			                           (rand() + 1) % frame_two_area->object->surface->buf->h);
	w_object_move(frame_two_area_white->object, (rand() + 1) % frame_two_area->object->surface->buf->w,
			                            (rand() + 1) % frame_two_area->object->surface->buf->h,
			                            (rand() + 1) % frame_two_area->object->surface->buf->w,
			                            (rand() + 1) % frame_two_area->object->surface->buf->h);
}

void frame_two_button_focus_pressed (w_object_t *object, int button)
{
	int r;
	w_frame_t *obj[] = {
		frame_two_area_red,
		frame_two_area_green,
		frame_two_area_blue,
		frame_two_area_white
	};
	r = rand() % 4;
	w_object_show(obj[r]->object);
}

void frame_three_button_add_pressed (w_object_t *object, int button)
{
	w_listbox_item_t *litem;
	if (frame_three_editbox->textbox->str) {
		w_listbox_item_init(frame_three_listbox->object, &litem);
		w_listbox_item_name_set(frame_three_listbox->object, litem, frame_three_editbox->textbox->str);
		w_listbox_item_add(frame_three_listbox->object, litem);
		w_scrollbuffer_slide(frame_three_listbox_scrollbuffer->object, 0, 0);
	}
}

void select_frame_one_button_pressed (w_object_t *object, int button)
{
	w_object_show(frame_one->object);
}

void select_frame_two_button_pressed (w_object_t *object, int button)
{
	w_object_show(frame_two->object);
}

void select_frame_three_button_pressed (w_object_t *object, int button)
{
	w_object_show(frame_three->object);
}

#if defined(CONFIG_SINGLE_APPLICATION)
int widgetx_main (int argc, char *argv[]);
s_single_app_t single_widgetx = {
	widgetx_main,
	1,
	{"widgetx"}
};
#endif
