#ifndef __TRANSITION_H__
#define __TRANSITION_H__

#define MAX_CUTOFF 1.0f
#define MIN_CUTOFF 0.0f

#include "SDL/include/SDL.h"
#include "App.h"
#include "Window.h"
#include "Render.h"
#include "Color.h"
#include "SceneManager.h"

enum class TransitionStep;

class Transition
{
public:
	Transition(float step_duration, bool non_lerp = false);
	virtual ~Transition();

	virtual void Start();
	virtual void CleanUp();														
	
	// Executes the transition
	virtual void DoTransition();

	float Lerp(float start, float end, float rate);									// Method that will be used to linearly interpolate the speed of a given transition.
	float N_Lerp(float start, float end, float rate, bool smash_in = false);		// Method that will be used to non-linearly interpolate the speed of a given transition.
	float GetCutoffRate(float step_duration, float dt = app->GetDT());				// Method that returns the cutoff rate for a given transition. Will use dt as the standard increase.

public:
															
	// The duration of each transtion step in seconds.
	float step_duration;

	// The current amount of accumulated cutoff.
	// Ranges from 0.0f to 1.0f. (0.0 = step start, 1.0f = step end)
	float current_cutoff;													

	// linearly or non-linearly interpolated.
	bool non_lerp;														

private:
	// The rate at which transition will elapse. See GetCutoffRate().
	float cutoff_rate;													
																					
};

#endif // !__TRANSITION_H__