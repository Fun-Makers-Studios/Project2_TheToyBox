#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

enum class GuiSliderType {
	FX,
	MUSIC
};

class GuiSlider : public GuiControl
{
public:

	GuiSlider(uint32 id, SDL_Rect bounds, const char* text, SDL_Rect sliderBounds);
	virtual ~GuiSlider();

	bool Start();

	bool Update(float dt);
	bool Draw(Render* render);

	bool CleanUp();
	int GetMusicValue();
	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	GuiSliderType slider;

private:

	SDL_Rect sliderbounds;

	float boundsX, boundsY;

	uint pressedFX, focusedFX;
};

#endif // __GUISLIDER_H__