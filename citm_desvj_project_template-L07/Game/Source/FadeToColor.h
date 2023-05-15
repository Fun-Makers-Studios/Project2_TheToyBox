#ifndef __FADE_TO_COLOR_H__
#define __FADE_TO_COLOR_H__

#include "Transition.h"
#include "Color.h"

class FadeToColor : public Transition
{
public:

	FadeToColor(Color colour);
	~FadeToColor();

	// Update the alpha value of the fade.
	void DoTransition();

	// Initialize all the variables related with the FadeToColor.
	void InitFadeToColour();												

private:

	SDL_Rect screen;
	Color fade_color;
};

#endif // !__FADE_TO_COLOR_H__