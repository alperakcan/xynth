#ifndef GUITEXTMATRIX_H_
#define GUITEXTMATRIX_H_

#include "GuiPrimitive.h"

/**
 * \brief Text matrix primitive
 *
 * This GUI primitive implements a multi-line text matrix. The specified font
 * is rendered onto the painting surface with selected size and color.
 * The matrix is divided into cells organized in rows and columns. Each
 * cell may have a specific style, which affects the foreground and background
 * colors of the text.
 *
 * XML code below defines a text matrix primitive:
 *
 * \code
 *	<item id="ID" type="text" x="X" y="Y" width="W" height="H">
 * 		<init font="F" size="S" rows="R" cols="C"
 *			static_text_color="RRGGBB"
 *			inactive_field_color="RRGGBB"
 *			active_field_color="RRGGBB"
 *			selection_color="RRGGBB"
 *			selection_bg_color="RRGGBB"
 *			/>
 * 		<touch x="tX" y="tY" width="tW" height="tH"> />
 * 	</item>
 * \endcode
 *
 * Where
 * - ID represents the primitive unique identifier
 * - X, Y, W, H is the set of primitive coordinates in the parent frame
 * - F specifies the font name that should match an available TTF font file name
 * - S specifies the font size of the text matrix
 * - R specifies the number of rows in the text matrix
 * - C specifies the number of columns in the text matrix
 * - RRGGBB values specify a number color parameters of the text matrix, encoded
 * as hex RGB (e.g. "6699cc"). Please refer to setCell() method for more
 * information.
 * - tX, tY, tW, tH are the coordinates of the touchable area in the parent frame
 *
 * Touchable area is optional and usually required only for button type elements.
 * Only static_text_color is a required color value, the rest are optional.
 *
 * \author Jaanus Sepp
 */

class GuiTextMatrix : public GuiPrimitive
{
public:

	/**
	 * \brief Cell type enumerator
	 *
	 * Specifies the type of the cell in the text matrix.
	 */
	enum CellType
	{
		/** Static text */
		CELL_STATIC_TEXT,
		/** Cell of an inactive field */
		CELL_INACTIVE_FIELD,
		/** Cell of an active field */
		CELL_ACTIVE_FIELD,
		/** Fully selected cell */
		CELL_FULL_SELECTION,
		/** Pre-selection cell */
		CELL_PRE_SELECTION,
		/** Post-selection cell */
		CELL_POST_SELECTION
	};

	GuiTextMatrix(int idResource, GuiFrame* pFrame, int width, int height);
	virtual ~GuiTextMatrix();

	bool setFont(const char* fontName, int fontSize);
	void setColor(const char *pStaticTextColor,
					const char *pInactiveFieldColor,
					const char *pActiveFieldColor,
					const char *pSelectionColor,
					const char *pSelectionBgColor);
	void setCell(size_t row, size_t col, const char* boxText, CellType cellType);
	int getCellHeight();
	bool setDimensions(int cols, int rows);
	int getColCount();
	int getRowCount();

protected:

	virtual void onPaint(GuiSurface *drawSurface, s_rect_t &paintArea);

private:

	s_font_t*	fontData;
	s_image_t*	imageData;

	void imageCopy(int destX, int destY, s_image_t *destImg,
					s_image_t *srcImg, int horOffset, int vertOffset);

	RGB_COLOR staticTextColor;
	RGB_COLOR inactiveFieldColor;
	RGB_COLOR activeFieldColor;
	RGB_COLOR selectionColor;
	unsigned int selectionBgColor;

	int cell_width;
	int cell_height;
	int col_count;
	int row_count;
	int img_width;
	int img_height;
};

#endif /*GUITEXTBOX_H_*/
