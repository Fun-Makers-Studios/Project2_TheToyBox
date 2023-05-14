#ifndef __TRANSITION_MANAGER_H__
#define __TRANSITION_MANAGER_H__

#include "Module.h"
#include "Color.h"
#include "Point.h"
//#include "Transition.h"


class		Transition;
enum class	SceneID;

class TransitionManager :  public Module
{
public:
	TransitionManager();
	~TransitionManager();

	bool PostUpdate();

	bool CleanUp();

public:
	void DeleteActiveTransition();
	
	Transition* CreateCut(SceneID next_scene);
	Transition* CreateFadeToColour(SceneID next_scene, float step_duration = 1.0f, Color fade_colour = Black);

	Transition* CreateSlide(SceneID next_scene, float step_duration = 0.5f, bool non_lerp = false, bool vertical = false,
									bool slide_from_right = false, bool slide_from_bottom = false, Color slide_colour = Black);

	Transition* CreateWipe(SceneID next_scene, float step_duration = 0.5f, bool non_lerp = false, bool vertical = false,
									bool wipe_from_right = false, bool wipe_from_bottom = false, Color wipe_colour = Black);

	Transition* CreateAlternatingBars(SceneID next_scene, float step_duration = 0.5f, bool non_lerp = false, int bar_number = 10, bool vertical = false,
										bool random_colours = false, Color even_color = Black, Color odd_color = Black);

	Transition* CreateExpandingBars(SceneID next_scene, float step_duration = 0.5f, bool non_lerp = false, int bar_number = 10, bool vertical = false,
										bool random_colours = false, Color even_color = Black, Color odd_color = Black);

	Transition* CreateZoomToMouse(SceneID next_scene, iPoint mouse_position, float step_duration = 0.5, bool non_lerp = false, float zoom_scale = 1.0f);
	Transition* CreateCameraToMouse(iPoint mouse_pos, float step_duration = 0.5f, bool non_lerp = false);


	// --- TRANSITION WITH TEXTURE CREATE METHODS
	Transition* CreateZoomToTexture(SceneID next_scene, iPoint mouse_position, float step_duration = 0.5f, float zoom_scale = 1.0f);
	Transition* CreateDissolve(SceneID next_scene, float step_duration = 0.5f);

public:
	bool						is_transitioning;																	// Will define whether a transition is active or not.

private:
	Transition*					active_transition;																	// Pointer to the transtion that's active at any given time.

};

#endif // !__TRANSITION_MANAGER_H__
