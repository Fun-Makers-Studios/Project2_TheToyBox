#ifndef __WIPE_H__
#define __WIPE_H__

#include "Transition.h"
#include "Color.h"

class Wipe : public Transition
{
public:

	Wipe(float step_duration, bool non_lerp, bool vertical, bool wipe_from_right, bool wipe_from_bottom, Color wipe_colour);
	~Wipe();

	void DoTransition();

	void HorizontalWipe();	// Will horizontally interpolate a wipe from left to right or right to left.
	void VerticalWipe();	// Will vertically interpolate a wipe from the top to bottom or bottom to top.
	void DrawWipe();

	void InitWipe();

private:

	SDL_Rect screen;
	Color wipe_colour;

	bool vertical;
	bool wipe_from_right;
	bool wipe_from_bottom;
};

#endif // !__WIPE_H__