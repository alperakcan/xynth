#ifndef GUIEDITBOX_H_
#define GUIEDITBOX_H_

#include "GuiPrimitive.h"

/**
 * \brief Edit box primitive
 * 
 * Edit box GUI primitive is not implemented.
 * 
 * \author Andrei Birjukov
 */

class GuiEditBox : public GuiPrimitive
{
public:
	GuiEditBox(int idResource, GuiFrame* pFrame);
	virtual ~GuiEditBox();
};

#endif /*GUIEDITBOX_H_*/
