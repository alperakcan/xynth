#ifndef GUIMULTITEXTBOX_H_
#define GUIMULTITEXTBOX_H_

#include <string>
#include <list>

#include "GuiPrimitive.h"

class GuiMultiTextBox : public GuiPrimitive
{
public:
	
	/**
	 * \brief Horizontal text alignment
	 * 
	 * Enum identifying the horizontal text alignment 
	 */
	enum TextHAlignment
	{
		/** Horizontal left alignment */
		LEFT, 
		/** Horizontal right alignment */
		RIGHT, 
		/** Horizontal center alignment */
		HCENTER 
	};
	
	GuiMultiTextBox(int idResource, GuiFrame* pFrame);
	virtual ~GuiMultiTextBox();

	void setFont(const char* fontName, int fontSize);
	void setColor(const char *pColor);
	void setText(const char* boxText);
	void setAlignment(TextHAlignment horz);
	
	void getRequiredSize(s_rect_t &rect);
	
protected:

	void renderText();

	virtual void onPaint(GuiSurface *drawSurface, s_rect_t &paintArea);
	
private:

	bool			isRendered;
	s_font_t**		fontData;
	int				numLines;
	
	std::string		fontName;
	std::list<std::string>	fontWords;
	RGB_COLOR		fontColor;
	int				fontSize;
	
	TextHAlignment	horzAlign;
};

#endif /*GUIMULTITEXTBOX_H_*/
