#ifndef GUITEXTMATRIX_H_
#define GUITEXTMATRIX_H_

#include "GuiPrimitive.h"

class GuiTextMatrix : public GuiPrimitive
{
public:

	enum CellType
	{
		CELL_STATIC_TEXT,
		CELL_INACTIVE_FIELD,
		CELL_ACTIVE_FIELD,
		CELL_FULL_SELECTION,
		CELL_PRE_SELECTION,
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
					s_image_t *srcImg, int vertOffset);

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
