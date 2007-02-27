
#include "../lib/xynth_.h"
#include "widget.h"

int w_listbox_item_image_init (w_listbox_item_image_t **item_image, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs)
{
	unsigned int i;
	w_listbox_item_image_t *ii;
	ii = (w_listbox_item_image_t *) s_malloc(sizeof(w_listbox_item_image_t));
	ii->style = style;
	ii->rotation = rotation;
	ii->nimages = nimgs;
	ii->images = (char **) s_malloc(sizeof(char *) * nimgs);
	for (i = 0; i < nimgs; i++) {
		ii->images[i] = strdup(imgs[i]);
	}
	*item_image = ii;
	return 0;
}

void w_listbox_item_image_uninit (w_listbox_item_image_t *item_image)
{
	unsigned int i;
	for (i = 0; i < item_image->nimages; i++) {
		s_free(item_image->images[i]);
	}
	s_free(item_image->images);
	s_free(item_image);
}

void w_listbox_item_data_set (w_listbox_item_t *listbox_item, void *data)
{
	listbox_item->data = data;
}

void w_listbox_item_name_set (w_object_t *listbox, w_listbox_item_t *listbox_item, char *name)
{
	s_free(listbox_item->name);
	listbox_item->name = strdup(name);
}

int w_listbox_item_init (w_object_t *listbox, w_listbox_item_t **listbox_item)
{
	int pos;
	w_listbox_t *lb;
	w_listbox_item_t *li;
	w_listbox_item_image_t *ii;
	lb = listbox->data[OBJECT_LISTBOX];
	li = (w_listbox_item_t *) s_malloc(sizeof(w_listbox_item_t));
	memset(li, 0, sizeof(w_listbox_item_t));
	w_textbox_init(listbox->window, &(li->textbox), listbox);
	w_textbox_set_size(li->textbox->object, lb->itemheight - 8);
	for (pos = 0; !s_list_eol(lb->item_images, pos); pos++) {
		ii = (w_listbox_item_image_t *) s_list_get(lb->item_images, pos);
		w_textbox_set_image(li->textbox->object, ii->style, ii->rotation, ii->nimages, ii->images);
	}
	w_object_show(li->textbox->object);
	*listbox_item = li;
	return 0;
}

void w_listbox_item_uninit (w_listbox_item_t *listbox_item)
{
	s_free(listbox_item->name);
	w_textbox_uninit(listbox_item->textbox->object);
	s_free(listbox_item);
}

int w_listbox_item_add (w_object_t *object, w_listbox_item_t *item)
{
	w_listbox_t *lb;
	w_listbox_item_t *active;
	lb = object->data[OBJECT_LISTBOX];
	active = s_list_get(lb->items, lb->active);
	s_list_add(lb->items, item, -1);
	if (active == NULL) {
		lb->active = 0;
	} else {
		lb->active = s_list_get_pos(lb->items, active);
	}
//	w_object_draw(object);
	return 0;
}

int w_listbox_item_del (w_object_t *object, w_listbox_item_t *item)
{
	int p;
	w_listbox_t *lb;
	w_listbox_item_t *active;
	lb = object->data[OBJECT_LISTBOX];
	active = s_list_get(lb->items, lb->active);
	p = s_list_get_pos(lb->items, item);
	s_list_remove(lb->items, p);
	w_listbox_item_uninit(item);
	if (active == NULL) {
		lb->active = 0;
	} else {
		lb->active = s_list_get_pos(lb->items, active);
	}
	if (lb->active < 0) {
		lb->active = 0;
	}
//	w_object_draw(object);
	return 0;
}

int w_listbox_clear (w_object_t *object)
{
	w_listbox_t *lb;
	w_listbox_item_t *li;
	lb = object->data[OBJECT_LISTBOX];
	while ((li = w_listbox_item_active_get(object)) != NULL) {
		w_listbox_item_del(object, li);
	}
	lb->height = 0;
	lb->yoffset = 0;
	lb->active = 0;
	lb->pactive = -1;
	return 0;
}

w_listbox_item_t * w_listbox_item_active_get (w_object_t *object)
{
	w_listbox_t *lb;
	lb = object->data[OBJECT_LISTBOX];
	return s_list_get(lb->items, lb->active); 
}

int w_listbox_item_active_set (w_object_t *object, w_listbox_item_t *listbox_item)
{
	int active;
	w_listbox_t *lb;
	lb = object->data[OBJECT_LISTBOX];
	active = s_list_get_pos(lb->items, listbox_item);
	if (active < 0) {
		return -1;
	}
	lb->active = active;
	w_object_draw(object);
	return 0;
}

void w_listbox_slide (w_object_t *object, int vertical, int horizontal, int *ytotal, int *yoffset)
{
	w_listbox_t *lb;
	lb = object->data[OBJECT_LISTBOX];
	lb->yoffset -= vertical;
	if (vertical > 0) {
		if (lb->active * lb->itemheight + lb->yoffset < 0) {
			lb->active += 1;
		}
	} else {
		if ((lb->active + 1) * lb->itemheight + lb->yoffset > lb->object->content->h) {
			lb->active -= 1;
		}
	}
	w_listbox_draw(object);
	//w_object_update(object, object->surface->win);
	(*ytotal) = lb->height;
	(*yoffset) = lb->yoffset;
}

void w_listbox_draw (w_object_t *object)
{
	int x;
	int y;
	int h;
	int w;
	int pos;
	w_listbox_t *lb;
	w_listbox_item_t *li;
	lb = object->data[OBJECT_LISTBOX];
	x = object->content->x;
	w = object->content->w;
	h = lb->itemheight;
	lb->height = lb->items->nb_elt * h;
	lb->yoffset = MAX(lb->yoffset, object->content->h - lb->height);
	lb->yoffset = MIN(0, lb->yoffset);
	y = object->content->y + lb->yoffset;
	if (object->surface->vbuf == NULL) {
		goto end;
	}
	if (lb->scrollbuffer) {
		if (((lb->active + 1) * h) + lb->yoffset > lb->object->content->h) {
			w_scrollbuffer_slide(lb->scrollbuffer, (((lb->active + 1) * h) + lb->yoffset) - lb->object->content->h, 0);
			return;
		}
		if ((lb->active * h) + lb->yoffset < 0) {
			w_scrollbuffer_slide(lb->scrollbuffer, ((lb->active * h) + lb->yoffset), 0);
			return;
		}
	}
	w_frame_draw(object);
	for (pos = 0; !s_list_eol(lb->items, pos); pos++, y += h) {
		li = (w_listbox_item_t *) s_list_get(lb->items, pos);
		if (!(pos == lb->active || pos == lb->pactive) &&
		    (li->textbox->object->surface->buf->x == x &&
		     li->textbox->object->surface->buf->y == y &&
		     li->textbox->object->surface->buf->w == w &&
		     li->textbox->object->surface->buf->h == h)) {
#if 0
			printf("pos %d, active: %d, pactive: %d\n"
			       "%d %d %d %d, %d %d %d %d\n",
			       pos, lb->active, lb->pactive, x, y, w, h,
			       li->textbox->object->surface->buf->x, li->textbox->object->surface->buf->y,
			       li->textbox->object->surface->buf->w, li->textbox->object->surface->buf->h);
#endif
			continue;
		}
		if (pos == lb->active) {
			w_textbox_set_rgb(li->textbox->object, 0, 0, 0);
			w_textbox_set_style(li->textbox->object, lb->activeshape, lb->activeshadow);
		} else {
			w_textbox_set_rgb(li->textbox->object, 255, 255, 255);
			w_textbox_set_style(li->textbox->object, lb->inactiveshape, lb->inactiveshadow);
		}
		w_textbox_set_str(li->textbox->object, li->name);
		w_object_move_silent(li->textbox->object, x, y, w, h);
	}
	if (lb->pactive != lb->active) {
		if (lb->changed != NULL) {
			lb->changed(object, lb->active);
		}
		lb->pactive = lb->active;
	}
	w_object_update(lb->object, lb->object->surface->win);
end:	return;
}

void w_listbox_geometry (w_object_t *object)
{
	w_frame_geometry(object);
	w_listbox_draw(object);
}

void w_listbox_event (w_object_t *object, s_event_t *event)
{
	w_listbox_t *lb;
	lb = object->data[OBJECT_LISTBOX];
	if (event->type & KEYBD_EVENT) {
		if (event->type & KEYBD_RELEASED) {
			if (event->keybd->keycode == S_KEYCODE_UP) {
				lb->active = MAX(lb->active - 1, 0);
				w_listbox_draw(object);
			} else if (event->keybd->keycode == S_KEYCODE_DOWN) {
				lb->active = MIN(lb->items->nb_elt - 1, lb->active + 1);
				lb->active = MAX(lb->active, 0);
				w_listbox_draw(object);
			}
		}
	}
}

void w_listbox_scrollbuffer_set (w_object_t *object, w_object_t *scrollbuffer)
{
	w_listbox_t *lb;
	lb = object->data[OBJECT_LISTBOX];
	lb->scrollbuffer = scrollbuffer;
}

int w_listbox_set_style (w_object_t *object, FRAME_SHAPE shape, FRAME_SHADOW shadow)
{
	return w_frame_set_style(object, shape, shadow);
}

int w_listbox_set_itemimage (w_object_t *object, unsigned int style, unsigned int rotation, unsigned int nimgs, char **imgs)
{
	w_listbox_t *lb;
	w_listbox_item_image_t *ii;
	lb = object->data[OBJECT_LISTBOX];
	w_listbox_item_image_init(&ii, style, rotation, nimgs, imgs);
	s_list_add(lb->item_images, ii, -1);
	return 0;
}

int w_listbox_set_active_style (w_object_t *object, unsigned int shape, unsigned int shadow)
{
	w_listbox_t *lb;
	w_listbox_item_t *li;
	lb = object->data[OBJECT_LISTBOX];
	lb->activeshape = shape;
	lb->activeshadow = shadow;
	li = w_listbox_item_active_get(object);
	if (li) {
		w_textbox_set_style(li->textbox->object, lb->activeshape, lb->activeshadow);
		w_object_draw(li->textbox->object);
	}
	return 0;
}

int w_listbox_set_inactive_style (w_object_t *object, unsigned int shape, unsigned int shadow)
{
	w_listbox_t *lb;
	lb = object->data[OBJECT_LISTBOX];
	lb->inactiveshape = shape;
	lb->inactiveshadow = shadow;
	w_object_draw(object);
	return 0;
}

int w_listbox_set_item_height (w_object_t *object, int size)
{
	w_listbox_t *lb;
	lb = object->data[OBJECT_LISTBOX];
	lb->itemheight = size;
	w_object_draw(object);
	return 0;
}

int w_listbox_init (w_window_t *window, w_listbox_t **listbox, w_object_t *parent)
{
	w_listbox_t *lb;
	lb = (w_listbox_t *) s_malloc(sizeof(w_listbox_t));
	w_frame_init(window, &(lb->frame), FRAME_PANEL | FRAME_SUNKEN, parent);
	s_list_init(&(lb->items));
	s_list_init(&(lb->item_images));
	lb->height = 0;
	lb->yoffset = 0;
	lb->active = 0;
	lb->pactive = -1;
	lb->itemheight = 24;
	lb->changed = NULL;
	lb->activeshape = FRAME_PANEL;
	lb->activeshadow = FRAME_SUNKEN;
	lb->inactiveshape = FRAME_NOFRAME;
	lb->inactiveshadow = FRAME_RAISED;
	lb->scrollbuffer = NULL;
	lb->object = lb->frame->object;
	lb->object->type = OBJECT_LISTBOX;
	lb->object->draw = w_listbox_draw;
	lb->object->event = w_listbox_event;
	lb->object->geometry = w_listbox_geometry;
	lb->object->destroy = w_listbox_uninit;
	lb->object->data[OBJECT_LISTBOX] = lb;
	*listbox = lb;
	return 0;
}

void w_listbox_uninit (w_object_t *object)
{
	w_listbox_t *lb;
	w_listbox_item_t *li;
	w_listbox_item_image_t *ii;
	lb = object->data[OBJECT_LISTBOX];
	while (!s_list_eol(lb->items, 0)) {
		li = (w_listbox_item_t *) s_list_get(lb->items, 0);
		s_list_remove(lb->items, 0);
		w_listbox_item_uninit(li);
	}
	while (!s_list_eol(lb->item_images, 0)) {
		ii = (w_listbox_item_image_t *) s_list_get(lb->item_images, 0);
		s_list_remove(lb->item_images, 0);
		w_listbox_item_image_uninit(ii);
	}
	s_list_uninit(lb->item_images);
	s_list_uninit(lb->items);
	w_frame_uninit(object);
	s_free(lb);
}
