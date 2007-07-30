/***************************************************************************
    begin                : Thu Jan 30 2003
    copyright            : (C) 2003 - 2007 by Alper Akcan
    email                : distchx@yahoo.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <xynth.h>
#include <widget.h>

#error "DEPRECATED"

#undef DEBUG

class Mines;

class MButton : public SButton {
public:
	int buttonR;
	int buttonC;
	Mines *buttonMines;
	
	void buttonPressed (int button);
	void buttonReleased (int button);
	
	void buttonDrawAll (void);
	void buttonDrawEmpty (int r, int c);
	
	void objectDraw (void);
	
	MButton (void);
	~MButton (void);
};

class Mines {
public:
	int minesC;
	int minesW;
	int minesH;

	s_image_t *minesFlag;
	s_image_t *minesMine;

	int **minesMatrix;
	int **minesMatrix_;
	
	SWindow *minesWindow;
	SLayout *topLayout;
	SLayout *btnsLayout;
	SLayout *minesLayout;
	
	int minesRand (int max);
	int minesInit (void);
	
	Mines (int arc, char *argv[]);
	~Mines (void);
};

void MButton::buttonPressed (int button)
{
}

void MButton::buttonReleased (int button)
{
        if (button == MOUSE_LEFTBUTTON) {
		buttonMines->minesMatrix_[buttonR][buttonC] = 1;
		if (buttonMines->minesMatrix[buttonR][buttonC] < 0) {
			buttonDrawAll();
		}
	} if (button == MOUSE_RIGHTBUTTON) {
		if (buttonMines->minesMatrix_[buttonR][buttonC] == 0) {
			buttonMines->minesMatrix_[buttonR][buttonC] = 2;
		} else if (buttonMines->minesMatrix_[buttonR][buttonC] == 2) {
			buttonMines->minesMatrix_[buttonR][buttonC] = 3;
		} else if (buttonMines->minesMatrix_[buttonR][buttonC] == 3) {
			buttonMines->minesMatrix_[buttonR][buttonC] = 0;
		}
	}
}

void MButton::buttonDrawAll (void)
{
	int r;
	int c;
	
	for (r = 0; r < buttonMines->minesH; r++) {
		for (c = 0; c < buttonMines->minesW; c++) {
			if (buttonMines->minesMatrix_[r][c] == 0) {
				buttonMines->minesMatrix_[r][c] = 1;
			}
		}
	}
	buttonMines->minesLayout->draw();
}

void MButton::buttonDrawEmpty (int r, int c)
{
	if ((r >= 0) && (r < buttonMines->minesH) &&
	    (c >= 0) && (c < buttonMines->minesW)) {
		if (buttonMines->minesMatrix_[r][c] == 0) {
			buttonMines->minesMatrix_[r][c] = 1;
			buttonMines->minesLayout->layoutCells[r][c].layoutCellObject->draw();
		}
	}
}

void MButton::objectDraw (void)
{
        int x;
        int y;
        int w;
        int h;
        int x_;
        int y_;
	int c[3];
        char *vbuf;
	char text[3];
        s_surface_t s;
	s_font_t *font;
	unsigned int *mat;
	
	if (buttonMines->minesMatrix_[buttonR][buttonC] == 1) {
		this->frameSetStyle((this->frameStyle & SFrame::MShape) | SFrame::Sunken);
	}
	
	SButton::objectDraw();

	if (buttonMines->minesMatrix_[buttonR][buttonC] == 1) {
		if (buttonMines->minesMatrix[buttonR][buttonC] == 0) {
			/* top */
			buttonDrawEmpty(buttonR - 1, buttonC);
			/* top right */
			buttonDrawEmpty(buttonR - 1, buttonC + 1);
			/* right */
			buttonDrawEmpty(buttonR, buttonC + 1);
			/* bottom right */
			buttonDrawEmpty(buttonR + 1, buttonC + 1);
			/* bottom */
			buttonDrawEmpty(buttonR + 1, buttonC);
			/* bottom left */
			buttonDrawEmpty(buttonR + 1, buttonC - 1);
			/* left */
			buttonDrawEmpty(buttonR, buttonC - 1);
			/* top left */
			buttonDrawEmpty(buttonR - 1, buttonC - 1);
		} else if (buttonMines->minesMatrix[buttonR][buttonC] > 0) {
			if ((objectRectContents.rectH - 2) < 0) {
				return;
			}
			s_font_init(&font, "arial.ttf");
			sprintf(text, "%d", (int) (buttonMines->minesMatrix[buttonR][buttonC]));
			s_font_set_str(font, text);
			s_font_set_size(font, objectRectContents.rectH - 2);
			s_font_get_glyph(font);
			s_image_get_handler(font->img);

			switch (buttonMines->minesMatrix[buttonR][buttonC]) {
				case 1: c[0] = 0;	c[1] = 0;	c[2] = 255;	break;
				case 2: c[0] = 0;	c[1] = 136;	c[2] = 0;	break;
				case 3: c[0] = 136;	c[1] = 136;	c[2] = 0;	break;
				case 4: c[0] = 136;	c[1] = 0;	c[2] = 136;	break;
				case 5: c[0] = 255;	c[1] = 0;	c[2] = 0;	break;
				case 6: c[0] = 136;	c[1] = 0;	c[2] = 0;	break;
				case 7: c[0] = 0;	c[1] = 0;	c[2] = 0;	break;
				default:
				case 8: c[0] = 0;	c[1] = 0;	c[2] = 0;	break;
			}

			x_ = objectRectContents.rectX + ((((objectRectContents.rectW - font->img->w) / 2) < 0) ? 0 : ((objectRectContents.rectW - font->img->w) / 2));
			y_ = objectRectContents.rectY + ((((objectRectContents.rectH - font->yMax) / 2) < 0) ? 0 : ((objectRectContents.rectH - font->yMax) / 2));
			w = (objectRectContents.rectW < font->img->w) ? objectRectContents.rectW : font->img->w;
			h = (objectRectContents.rectH < font->img->h) ? objectRectContents.rectH : font->img->h;

		        vbuf = (char *) s_malloc(objectWindow->surface->bytesperpixel * font->img->w * font->img->h + 1);
		        s_getsurfacevirtual(&s, font->img->w, font->img->h, objectWindow->surface->bitsperpixel, vbuf);
		        s_getbox(objectWindow->surface, x_, y_, font->img->w, font->img->h, s.vbuf);

		        mat = font->img->rgba;
			for (y = 0; y < h; y++) {
				for (x = 0; x < w; x++) {
					if (~*mat & 0xff) {
						s_setpixelrgba(&s, x, y, c[0], c[1], c[2], *mat & 0xff);
					}
					mat++;
				}
				mat += font->img->w - w;
			}

			s_putbox(objectWindow->surface, x_, y_, font->img->w, font->img->h, s.vbuf);
        		s_free(vbuf);
        		s_font_uninit(font);
		} else if (buttonMines->minesMatrix[buttonR][buttonC] < 0) {
			x = objectRectContents.rectX + ((((objectRectContents.rectW - buttonMines->minesMine->w) / 2) < 0) ? 0 : ((objectRectContents.rectW - buttonMines->minesMine->w) / 2));
			y = objectRectContents.rectY + ((((objectRectContents.rectH - buttonMines->minesMine->h) / 2) < 0) ? 0 : ((objectRectContents.rectH - buttonMines->minesMine->h) / 2));
			w = (objectRectContents.rectW < buttonMines->minesMine->w) ? objectRectContents.rectW : buttonMines->minesMine->w;
			h = (objectRectContents.rectH < buttonMines->minesMine->h) ? objectRectContents.rectH : buttonMines->minesMine->h;
			s_putboxpartmask(objectWindow->surface, x, y, w, h, buttonMines->minesMine->w, buttonMines->minesMine->h, buttonMines->minesMine->buf, buttonMines->minesMine->mat, 0, 0);
		}
	} else 	if (buttonMines->minesMatrix_[buttonR][buttonC] == 2) {
		x = objectRectContents.rectX + ((((objectRectContents.rectW - buttonMines->minesFlag->w) / 2) < 0) ? 0 : ((objectRectContents.rectW - buttonMines->minesFlag->w) / 2));
		y = objectRectContents.rectY + ((((objectRectContents.rectH - buttonMines->minesFlag->h) / 2) < 0) ? 0 : ((objectRectContents.rectH - buttonMines->minesFlag->h) / 2));
		w = (objectRectContents.rectW < buttonMines->minesFlag->w) ? objectRectContents.rectW : buttonMines->minesFlag->w;
		h = (objectRectContents.rectH < buttonMines->minesFlag->h) ? objectRectContents.rectH : buttonMines->minesFlag->h;
		s_putboxpartmask(objectWindow->surface, x, y, w, h, buttonMines->minesFlag->w, buttonMines->minesFlag->h, buttonMines->minesFlag->buf, buttonMines->minesFlag->mat, 0, 0);
	} else 	if (buttonMines->minesMatrix_[buttonR][buttonC] == 3) {
		if ((objectRectContents.rectH - 2) < 0) {
			return;
		}
		s_font_init(&font, "arial.ttf");
		sprintf(text, "%s", "?");
		s_font_set_str(font, text);
		s_font_set_size(font, objectRectContents.rectH - 2);
		s_font_get_glyph(font);
		s_image_get_handler(font->img);

		x_ = objectRectContents.rectX + ((((objectRectContents.rectW - font->img->w) / 2) < 0) ? 0 : ((objectRectContents.rectW - font->img->w) / 2));
		y_ = objectRectContents.rectY + ((((objectRectContents.rectH - font->yMax) / 2) < 0) ? 0 : ((objectRectContents.rectH - font->yMax) / 2));
		w = (objectRectContents.rectW < font->img->w) ? objectRectContents.rectW : font->img->w;
		h = (objectRectContents.rectH < font->img->h) ? objectRectContents.rectH : font->img->h;
                
		vbuf = (char *) s_malloc(objectWindow->surface->bytesperpixel * font->img->w * font->img->h + 1);
		s_getsurfacevirtual(&s, font->img->w, font->img->h, objectWindow->surface->bitsperpixel, vbuf);
		s_getbox(objectWindow->surface, x_, y_, font->img->w, font->img->h, s.vbuf);

                mat = font->img->rgba;
		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				if (~*mat & 0xff) {
					s_setpixelrgba(&s, x, y, 0, 0, 0, *mat & 0xff);
				}
				mat++;
			}
			mat += font->img->w - w;
		}

		s_putbox(objectWindow->surface, x_, y_, font->img->w, font->img->h, s.vbuf);
        	s_free(vbuf);
		s_font_uninit(font);
	}
}

MButton::MButton (void) : SButton(NULL, SFrame::Panel)
{
}

MButton::~MButton (void)
{
}

int Mines::minesRand (int max)
{
	return ((int) (((float) max * rand() / RAND_MAX * 1.0)));
}

int Mines::minesInit (void)
{
	int i;
	int m;
	int x;
	int y;
	int mx;
	int my;
	
	for (y = 0; y < minesH; y++) {
		for (x = 0; x < minesW; x++) {
			minesMatrix[y][x] = 0;
			minesMatrix_[y][x] = 0;
		}
	}
	
	srand(time(NULL));
	
	for (i = 0; i < minesC; i++) {
		m = minesRand(minesW * minesH);
		my = m / minesW;
		mx = m % minesW;
		if (minesMatrix[my][mx] == -1) {
			i--;
			continue;
		} else {
			minesMatrix[my][mx] = -1;
		}
	}
	
	for (y = 0; y < minesH; y++) {
		for (x = 0; x < minesW; x++) {
			i = 0;
			// top
			if (y > 0) {
				if (minesMatrix[y - 1][x] == -1) {
					i++;
				}
			}
			// top left
			if ((y > 0) && (x > 0)) {
				if (minesMatrix[y - 1][x - 1] == -1) {
					i++;
				}
			}
			// left
			if (x > 0) {
				if(minesMatrix[y][x - 1] == -1) {
					i++;
				}
			}
			// buttom left
			if ((y < (minesH - 1)) && (x > 0)) {
				if (minesMatrix[y + 1][x - 1] == -1) {
					i++;
				}
			}
			// buttom
			if (y < (minesH - 1)) {
				if(minesMatrix[y + 1][x] == -1) {
					i++;
				}
			}
			// buttom right
			if ((y < (minesH - 1)) && (x < (minesW - 1))) {
				if(minesMatrix[y + 1][x + 1] == -1) {
					i++;
				}
			}
			// right
			if (x < (minesW - 1)) {
				if (minesMatrix[y][x + 1] == -1) {
					i++;
				}
			}
			// top right
			if ((y > 0) && (x < (minesW - 1))) {
				if (minesMatrix[y - 1][x + 1] == -1) {
					i++;
				}
			}
			if(minesMatrix[y][x] == 0) {
				minesMatrix[y][x] = i;
			}
		}
	}
	
	return 0;
}

Mines::Mines (int argc, char *argv[])
{
        int r;
        int c;
        int o;
        char *tmp;
	SButton *sbutton;
	MButton *mbutton;
        unsigned int *rgba;
        char *flag_img = {"        .     "
        	          "....... .     "
	                  ".,,,,,. .     "
	                  ".,,,,,. .     "
	                  ".,,,,,. .     "
	                  ".,,,,,. .     "
	                  "....... .     "
	                  "        .     "
	                  "        .     "
	                  "        .     "
	                  "        .     "
	                  "      ....    "
	                  "     ......   "
	                  "   ..........."};
	char *mine_img = {"      .       "
        	          "      .       "
	                  " .  .....     "
	                  "  .........   "
	                  "  .........   "
	                  " ...,,......  "
	                  " ...,,......  "
	                  ".............."
	                  " ...........  "
	                  " ...........  "
	                  "  .........   "
	                  "  .........   "
	                  " .  .....  .  "
	                  "      .       "};

	minesC = 40;
	minesW = 17;
	minesH = 17;

	while ((o = getopt(argc, argv, "m:w:h:")) != -1) {
		switch (o) {
			case 'm':
				minesC = atoi(optarg);
				break;
			case 'w':
				minesW = atoi(optarg);
				break;
			case 'h':
				minesH = atoi(optarg);
				break;
			default:
				break;
		}
	}

	minesMatrix = new int*[minesH];
	minesMatrix_ = new int*[minesH];
	for (o = 0; o < minesH; o++) {
		minesMatrix[o] = new int[minesW];
		minesMatrix_[o] = new int[minesW];
	}
	
	minesWindow = new SWindow();

	s_image_init(&minesFlag);
	minesFlag->rgba = (unsigned int *) s_malloc(14 * 14 * sizeof(unsigned int));
	tmp = flag_img;
	rgba = minesFlag->rgba;
	minesFlag->w = 14;
	minesFlag->h = 14;
	for (r = 0; r < 14; r++) {
		for (c = 0; c < 14; c++) {
			if (*tmp == '.') {
				*rgba = 0x00000000;
			} else if (*tmp == ',') {
				*rgba = 0xFF000000;
			} else {
				*rgba = 0x000000FF;
			}
			tmp++;
			rgba++;
		}
	}
	s_image_get_mat(minesFlag);
	s_image_get_buf(minesWindow->objectWindow->surface, minesFlag);
	
	s_image_init(&minesMine);
	minesMine->rgba = (unsigned int *) s_malloc(14 * 14 * sizeof(unsigned int));
	tmp = mine_img;
	rgba = minesMine->rgba;
	minesMine->w = 14;
	minesMine->h = 14;
	for (r = 0; r < 14; r++) {
		for (c = 0; c < 14; c++) {
			if (*tmp == '.') {
				*rgba = 0x00000000;
			} else if (*tmp == ',') {
				*rgba = 0xFFFFFF00;
			} else {
				*rgba = 0x000000FF;
			}
			tmp++;
			rgba++;
		}
	}
	s_image_get_mat(minesMine);
	s_image_get_buf(minesWindow->objectWindow->surface, minesMine);
	
	topLayout = new SLayout(minesWindow, 1, 1);
	topLayout->layoutSetColSpacing(10);
	topLayout->layoutSetRowSpacing(10);
	
	btnsLayout = new SLayout(NULL, 1, 3);
	btnsLayout->layoutSetColSpacing(5);
	btnsLayout->layoutSetRowSpacing(5);
	
	minesLayout = new SLayout(NULL, minesH, minesW);
	minesLayout->layoutSetColSpacing(1);
	minesLayout->layoutSetRowSpacing(1);

//	topLayout->layoutInsertObject(btnsLayout, 0, 0);
	topLayout->layoutInsertObject(minesLayout, 0, 0);
	
	sbutton = new SButton(NULL, SFrame::Panel);
	btnsLayout->layoutInsertObject(sbutton, 0, 1);
	
	for (r = 0; r < minesH; r++) {
		for (c = 0; c < minesW; c++) {
			mbutton = new MButton();
			mbutton->buttonR = r;
			mbutton->buttonC = c;
			mbutton->buttonMines = this;
			minesLayout->layoutInsertObject(mbutton, r, c);
		}
	}
	
	minesWindow->windowSetCoor(0, 0, minesW * 25, minesH * 25);
	minesWindow->windowSetTitle("Demo - %s", argv[0]);
	
	minesInit();

#ifdef DEBUG
	for (r = 0; r < minesH; r++) {
		for (c = 0; c < minesW; c++) {
			printf("%3d", minesMatrix[r][c]);
		}
		printf("\n");
	}
#endif
	
	minesWindow->windowShow();
	minesWindow->windowMain();
}

Mines::~Mines (void)
{
	while (minesH--) {
		delete[] minesMatrix[minesH];
		delete[] minesMatrix_[minesH];
	}

	delete minesMatrix;
	delete minesMatrix_;
	
	s_image_uninit(minesFlag);
	s_image_uninit(minesMine);

	delete minesWindow;
}

int main (int argc, char *argv[])
{
	Mines *mines;
	
	mines = new Mines(argc, argv);
	
	delete mines;
	
	return 0;
}
