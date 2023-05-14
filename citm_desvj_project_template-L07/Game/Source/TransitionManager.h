#ifndef __TRANSITION_MANAGER_H__
#define __TRANSITION_MANAGER_H__

#include "Module.h"
#include "Color.h"
#include "Point.h"
//#include "Transition.h"


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

public:

	void StepTransition();

	void DeleteActiveTransition();
	
	Transition* CreateFadeToColour(float step_duration = 1.0f, Color fade_colour = Black);
	Transition* CreateSlide(float step_duration = 0.5f, bool non_lerp = false, bool vertical = false, bool slide_from_right = false, bool slide_from_bottom = false, Color slide_colour = Black);
	Transition* CreateWipe(float step_duration = 0.5f, bool non_lerp = false, bool vertical = false, bool wipe_from_right = false, bool wipe_from_bottom = false, Color wipe_colour = Black);
	Transition* CreateAlternatingBars(float step_duration = 0.5f, bool non_lerp = false, int bar_number = 10, bool vertical = false, bool random_colours = false, Color even_color = Black, Color odd_color = Black);
	Transition* CreateExpandingBars(float step_duration = 0.5f, bool non_lerp = false, int bar_number = 10, bool vertical = false, bool random_colours = false, Color even_color = Black, Color odd_color = Black);


	// --- TRANSITION WITH TEXTURE CREATE METHODS
	Transition* CreateZoomToTexture(iPoint mouse_position, float step_duration = 0.5f, float zoom_scale = 1.0f);
	Transition* CreateDissolve(float step_duration = 0.5f);

public:

	// Pointer to the transtion that's active at any given time.
	Transition* currentTransition = nullptr;

	// All the steps that a given transition will go through.
	TransitionStep step = TransitionStep::NONE;
};

#endif // !__TRANSITION_MANAGER_H__
