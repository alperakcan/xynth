#ifndef GUISURFACE_H_
#define GUISURFACE_H_

#include <xynth.h>

struct RGB_COLOR
{
	int red;
	int green;
	int blue;
};

/**
 * \brief Graphical window surface
 * 
 * Given class implements the graphical window surface by wrapping the surface
 * API of the Xynth engine. A new virtual surface can be instantiated, or
 * the object can be attached to an existing surface. Object methods may be
 * then used to draw pixels, lines, boxes and copy rectangular areas from
 * another surfaces.
 * 
 * \author Andrei Birjukov, Anti Sullin
 */

class GuiSurface
{
public:
	GuiSurface(int width, int height, int bpp);
	GuiSurface(s_surface_t *pSurface);
	virtual ~GuiSurface();

	void drawImage(int x, int y, s_image_t *pBitmap);
	void drawImagePart(int destX, int destY, s_rect_t *imgPart, s_image_t *pBitmap);

	void fillRect(s_rect_t *pRect, int rgb);
	void setPixel(int x, int y, int rgb);
	void drawLine(int x1, int y1, int x2, int y2, int rgb);
	void drawCircle(int x1, int y1, int radius, int rgb);
	void drawPrimitive(int x1, int y1);
	
	int makeColor(int r, int g, int b);
	int makeColor(RGB_COLOR *rgb);
	static RGB_COLOR stringToRGB(const char *str);
	
	void copySurface(GuiSurface *pSrc);
	void copySurface(int x, int y, GuiSurface *pSrc);
	void copySurfacePart(int destX, int destY, s_rect_t *srcPart, GuiSurface *pSrc);
	
	int getWidth();
	int getHeight();
	int getBitsPerPixel();
	
private:

	s_surface_t *pSurface;
	char *virtBuf;
};

#endif /*GUISURFACE_H_*/
