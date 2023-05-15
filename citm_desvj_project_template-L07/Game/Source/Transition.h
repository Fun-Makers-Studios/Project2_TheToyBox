#ifndef __TRANSITION_H__
#define __TRANSITION_H__

#include "SDL/include/SDL.h"
#include "App.h"
#include "Window.h"
#include "Render.h"
#include "Color.h"
#include "SceneManager.h"

enum class TransitionStart
{
	TOP,
	BOT,
	LEFT,
	RIGHT
};

enum class TransitionStep;

class Transition
{
public:
	
	Transition() {}
	virtual ~Transition() {}

	virtual void Start() {}
	virtual void CleanUp() {}
	
	// Executes the transition
	virtual void DoTransition() {}

};

#endif // !__TRANSITION_H__