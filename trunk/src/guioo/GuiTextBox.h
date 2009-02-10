#ifndef GUITEXTBOX_H_
#define GUITEXTBOX_H_

#include <string>

#include "GuiPrimitive.h"

/**
 * \brief Textbox primitive
 *
 * Given GUI primitive implements a single-line textbox. The specified font
 * is rendered onto the painting surface with selected size and color.
 * The text alignment can be also specified both vertically and horizontally.
 *
 * XML code below defines a text box primitive:
 *
 * \code
 *	<item id="ID" type="text" x="X" y="Y" width="W" height="H">
 * 		<init text="T" font="F" size="S" color="RRGGBB" align="HV" />
 * 		<touch x="tX" y="tY" width="tW" height="tH"> />
 * 	</item>
 * \endcode
 *
 * Where
 * - ID represents the primitive unique identifier
 * - X, Y, W, H is the set of primitive coordinates in the parent frame
 * - T contains the initial text of the text box
 * - F specifies the font name that should match an available TTF font file name
 * - S specifies the font size
 * - RRGGBB specifies the font color encoded as hex RGB (e.g. "6699cc")
 * - HV sets the text alignment within the box coordinates. The first character
 *  specifies the horizontal alignment, the second character specifies the
 *	vertical alignment. Available options are left ("l"), right ("r") and
 * 	center ("c"). For example, "lc" is the horizontal left and vertical center.
 * - tX, tY, tW, tH are the coordinates of the touchable area in the parent frame
 *
 * Touchable area is optional and usually required only for button type elements.
 *
 * \author Andrei Birjukov
 */

class GuiTextBox : public GuiPrimitive
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

	/**
	 * \brief Vertical text alignment
	 *
	 * Enum identifying the horizontal text alignment
	 */
	enum TextVAlignment
	{
		/** Vertical top alignment */
		TOP,
		/** Vertical bottom alignment */
		BOTTOM,
		/** Vertical center alignment */
		VCENTER,
		/** Vertical normal (based on baseline) alignment */
		VNORMAL
	};

	/**
	 * \brief Font style identifier
	 *
	 * Enum identifying the font style, being a separate TTF
	 */
	enum FontStyle
	{
		/** Regular font style */
		REGULAR,
		/** Bold font style */
		BOLD,
		/** Italic font style */
		ITALIC
	};

	/**
	 * \brief Formatted (rich) line of text
	 *
	 * Structure that defines a rich line of text and contains font size, style,
	 * color, text alignment, along with the actual text.
	 */
	struct FormattedTextLine
	{
		/** Identifier of the text line, optional */
		int					lineId;
		/** Horizontal alignment of text */
		TextHAlignment		textAlign;
		/** Specifies the style of font: bold, italic, regular */
		FontStyle			fontStyle;
		/** Specifies the size of font, in pixels */
		int					fontSize;
		/** Specifies the color of font, RGB structure */
		RGB_COLOR			fontColor;

		/** Displayed text string in UTF-8 format */
		std::string 		lineText;
	};

	GuiTextBox(int idResource, GuiFrame* pFrame);
	virtual ~GuiTextBox();

	bool setFont(const char* fontName, int fontSize);
	void setColor(const char *pColor);
	void setColor(RGB_COLOR *pColor);
	void setAlignment(TextHAlignment horz, TextVAlignment vert);

	void setText(const char* boxText);
	bool setText(FormattedTextLine *pText);

	void getRequiredSize(s_rect_t &rect);

protected:

	virtual void onPaint(GuiSurface *drawSurface, s_rect_t &paintArea);

private:

	s_font_t*		fontData;
	TextHAlignment	horzAlign;
	TextVAlignment	vertAlign;
};

#endif /*GUITEXTBOX_H_*/
