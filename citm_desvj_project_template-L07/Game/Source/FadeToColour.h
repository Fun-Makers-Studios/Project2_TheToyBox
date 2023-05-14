#ifndef __FADE_TO_COLOUR_H__
#define __FADE_TO_COLOUR_H__

#include "Transition.h"
#include "Color.h"

class FadeToColour : public Transition
{
public:

	FadeToColour(float step_duration, Color colour);
	~FadeToColour();

	// Update the alpha value of the fade.
	void DoTransition();

	// Initialize all the variables related with the FadeToColour.
	void InitFadeToColour();												

private:

	SDL_Rect screen;
	Color fade_colour;
};

#endif // !__FADE_TO_COLOUR_H__