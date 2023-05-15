#ifndef __TRANSITION_MANAGER_H__
#define __TRANSITION_MANAGER_H__

#include "Module.h"
#include "Color.h"
#include "Point.h"
#include "Easing.h"
#include "Transition.h"


enum class TransitionStep
{
	NONE,
	IN,
	SWITCH,
	OUT,
	FINISHED
};

class Transition;
enum class SceneID;

class TransitionManager :  public Module
{
public:

	TransitionManager();
	~TransitionManager();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	void StepTransition();
	void LoadTransition();
	void DeleteActiveTransition();
	
	Transition* CreateFadeToColour(Color fade_color = Black);
	Transition* CreateSlide(TransitionStart transitionStart = TransitionStart::LEFT, Color slide_colour = Black);
	Transition* CreateWipe(TransitionStart transitionStart = TransitionStart::LEFT, Color wipe_colour = Black);
	Transition* CreateAlternatingBars(int bar_number, bool vertical, bool random_colours, Color even_color = Black, Color odd_color = Black);
	Transition* CreateExpandingBars(int bar_number = 10, bool vertical = false, bool random_colours = false, Color even_color = Black, Color odd_color = Black);

	// --- TRANSITION WITH TEXTURE CREATE METHODS
	Transition* CreateZoomToTexture(iPoint mouse_position, float zoom_scale = 1.0f);
	Transition* CreateDissolve(float step_duration = 0.5f);

public:

	// Pointer to the transtion that's active at any given time.
	Transition* currentTransition = nullptr;

	// All the steps that a given transition will go through.
	TransitionStep step = TransitionStep::NONE;

	Easing* easing = nullptr;
	double trackedTime = 0.0f;

	EasingType easingTypeIN;
	EasingType easingTypeOUT;
};

#endif // !__TRANSITION_MANAGER_H__