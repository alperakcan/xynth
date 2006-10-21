#include <stdio.h>
#include <stdlib.h>
#include <xynth.h>
#include <widget.h>

w_window_t *demo;
w_frame_t *frame;
w_button_t *button_hide;
w_textbox_t *textbox_hide;
w_button_t *button_blender;
w_textbox_t *textbox_blender;
w_button_t *button_focus;
w_textbox_t *textbox_focus;
w_checkbox_t *checkbox;
w_frame_t *area;
w_frame_t *red;
w_frame_t *green;
w_frame_t *blue;
w_frame_t *white;
void red_draw (w_object_t *object);
void green_draw (w_object_t *object);
void blue_draw (w_object_t *object);
void white_draw (w_object_t *object);

int main (int argc, char *argv[])
{
srand(time(NULL));
w_window_init(&demo, WINDOW_MAIN, NULL);
s_window_set_title(demo->window, "Object XML Demo");
w_window_set_coor(demo, 60, 60, 500, 400);
w_frame_init(demo, &frame, 0, demo->object);
frame->style = FRAME_PANEL | FRAME_RAISED;
w_object_move(frame->object, 0, 0, 500, 600);
w_object_show(frame->object);
w_button_init(demo, &button_hide, frame->object);
w_object_move(button_hide->object, 5, 5, 70, 20);
w_object_show(button_hide->object);
w_textbox_init(demo, &textbox_hide, button_hide->object);
textbox_hide->frame->style = FRAME_NOFRAME | 0;
w_textbox_set_str(textbox_hide->object, "hide area");
w_object_move(textbox_hide->object, 0, 0, 70, 20);
w_object_show(textbox_hide->object);
w_button_init(demo, &button_blender, frame->object);
w_object_move(button_blender->object, 80, 5, 55, 20);
w_object_show(button_blender->object);
w_textbox_init(demo, &textbox_blender, button_blender->object);
textbox_blender->frame->style = FRAME_NOFRAME | 0;
w_textbox_set_str(textbox_blender->object, "blender");
w_object_move(textbox_blender->object, 0, 0, 55, 20);
w_object_show(textbox_blender->object);
w_button_init(demo, &button_focus, frame->object);
w_object_move(button_focus->object, 140, 5, 85, 20);
w_object_show(button_focus->object);
w_textbox_init(demo, &textbox_focus, button_focus->object);
textbox_focus->frame->style = FRAME_NOFRAME | 0;
w_textbox_set_str(textbox_focus->object, "random focus");
w_object_move(textbox_focus->object, 0, 0, 85, 20);
w_object_show(textbox_focus->object);
w_checkbox_init(demo, &checkbox, frame->object);
w_object_move(checkbox->object, 300, 5, 85, 20);
w_textbox_set_str(checkbox->text->object, "checkbox");
w_object_show(checkbox->object);
w_frame_init(demo, &area, 0, frame->object);
area->style = FRAME_PANEL | FRAME_SUNKEN;
w_object_move(area->object, 5, 30, 490, 300);
w_frame_init(demo, &red, 0, area->object);
red->style = FRAME_PANEL | FRAME_PLAIN;
red->object->draw = red_draw;
w_object_move(red->object, (rand() + 1) % 490, (rand() + 1) % 300, (rand() + 1) % 490, (rand() + 1) % 300);
w_object_show(red->object);
w_frame_init(demo, &green, 0, area->object);
green->style = FRAME_PANEL | FRAME_PLAIN;
green->object->draw = green_draw;
w_object_move(green->object, (rand() + 1) % 490, (rand() + 1) % 300, (rand() + 1) % 490, (rand() + 1) % 300);
w_object_show(green->object);
w_frame_init(demo, &blue, 0, area->object);
blue->style = FRAME_PANEL | FRAME_PLAIN;
blue->object->draw = blue_draw;
w_object_move(blue->object, (rand() + 1) % 490, (rand() + 1) % 300, (rand() + 1) % 490, (rand() + 1) % 300);
w_object_show(blue->object);
w_frame_init(demo, &white, 0, area->object);
white->style = FRAME_PANEL | FRAME_PLAIN;
white->object->draw = white_draw;
w_object_move(white->object, (rand() + 1) % 490, (rand() + 1) % 300, (rand() + 1) % 490, (rand() + 1) % 300);
w_object_show(white->object);
w_object_show(area->object);
w_object_show(demo->object);
s_window_show(demo->window);
s_client_main(demo->window);
return 0;
}
