#ifndef __SLIDE_H__
#define __SLIDE_H__

#include "Transition.h"
#include "Color.h"

class Slide : public Transition
{
public:

	Slide(float step_duration, bool non_lerp, bool vertical, bool slide_from_right, bool slide_from_bottom, Color slide_colour);
	~Slide();

	void DoTransition();

	void HorizontalSlide();		// Will horizontally interpolate a slide from left to right or right to left.
	void VerticalSlide();		// Will vertically interpolate a slide from the top to bottom or bottom to top.
	void DrawSlide();

	void InitSlide();

private:

	SDL_Rect screen;
	Color slide_colour;

	bool vertical;		
	bool slide_from_right;
	bool slide_from_bottom;
};

#endif // !__SLIDE_H__