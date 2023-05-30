#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiButton : public GuiControl
{
public:

	GuiButton(uint32 id, SDL_Rect bounds, const char* text, int textSize, ButtonType buttonType_ = ButtonType::RECTANGLE_M);
	virtual ~GuiButton();

	bool Update(float dt);
	bool Draw(Render* render);

	SDL_Rect GetButtonRect(GuiControlState guiControlState, ButtonType buttonType);

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	SDL_Texture* buttonTex;
	const char* buttonTexPath;

	ButtonType buttonType;
};

#endif // __GUIBUTTON_H__