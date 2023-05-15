#ifndef __SLIDE_H__
#define __SLIDE_H__

#include "Transition.h"
#include "Color.h"

class Slide : public Transition
{
public:

	Slide(TransitionStart transitionStart, Color slide_colour);
	~Slide();

	void InitSlide();
	void DoTransition();
	void DrawSlide();

private:

	SDL_Rect screen;
	Color slide_colour;

	TransitionStart transitionStart;
};

#endif // !__SLIDE_H__