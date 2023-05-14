#ifndef __GUIINVENTORYSLOT_H__
#define __GUIINVENTORYSLOT_H__

#include "GuiControl.h"
#include "Item.h"

#include "Point.h"
#include "SString.h"

class GuiInventorySlot : public GuiControl
{
public:

	GuiInventorySlot(uint32 id, SDL_Rect bounds, const char* text, int textSize);
	virtual ~GuiInventorySlot();

	bool Update(float dt);
	bool Draw(Render* render);

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	SDL_Texture* slotTex;
	const char* slotTexPath;

	uint slotID = 0;

	Item* slotItem = nullptr; 
	bool selected = false;
};

#endif // __GUIBUTTON_H__