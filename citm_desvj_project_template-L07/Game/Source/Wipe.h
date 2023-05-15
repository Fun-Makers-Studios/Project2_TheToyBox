#ifndef __WIPE_H__
#define __WIPE_H__

#include "Transition.h"
#include "Color.h"

class Wipe : public Transition
{
public:

	Wipe(TransitionStart transitionStart, Color wipe_colour);
	~Wipe();

	void InitWipe();
	void DoTransition();
	void DrawWipe();

private:

	SDL_Rect screen;
	Color wipe_colour;

	TransitionStart transitionStart;
};

#endif // !__WIPE_H__