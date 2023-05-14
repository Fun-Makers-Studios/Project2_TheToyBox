#include "TransitionManager.h"
#include "FadeToColour.h"
#include "Slide.h"
#include "Wipe.h"
#include "AlternatingBars.h"
#include "ExpandingBars.h"
#include "ZoomToTexture.h"
#include "Dissolve.h"

TransitionManager::TransitionManager() : currentTransition(nullptr)
{

}

TransitionManager::~TransitionManager()
{

}

bool TransitionManager::PreUpdate()
{
	return true;
}

bool TransitionManager::Update(float dt)
{
	return true;
}

bool TransitionManager::PostUpdate()
{
	bool ret = true;

	if (currentTransition != nullptr)
	{
		StepTransition();
	}

	return ret;
}

bool TransitionManager::CleanUp()
{
	bool ret = true;
	
	if (currentTransition != nullptr)
	{
		delete currentTransition;
		currentTransition = nullptr;
	}

	return ret;
}

void TransitionManager::StepTransition()
{
	switch (step)
	{
		case TransitionStep::IN:

			currentTransition->current_cutoff += 0.016; // HEKATE!!! /*currentTransition->GetCutoffRate(currentTransition->step_duration)*/

			if (currentTransition->current_cutoff >= MAX_CUTOFF)
			{
				currentTransition->current_cutoff = MAX_CUTOFF;
				app->transitionManager->step = TransitionStep::SWITCH;
			}

			break;

		case TransitionStep::SWITCH:

			app->transitionManager->step = TransitionStep::OUT;

			break;

		case TransitionStep::OUT:

			currentTransition->current_cutoff -= 0.016; // HEKATE!!! /*currentTransition->GetCutoffRate(currentTransition->step_duration)*/

			if (currentTransition->current_cutoff <= MIN_CUTOFF)
			{
				currentTransition->current_cutoff = MIN_CUTOFF;
				app->transitionManager->DeleteActiveTransition();
				app->transitionManager->step = TransitionStep::FINISHED;
			}

			break;
	}

	if (app->transitionManager->step != TransitionStep::FINISHED)
	{
		currentTransition->DoTransition();
	}
}

void TransitionManager::DeleteActiveTransition()
{
	delete currentTransition;
	currentTransition = nullptr;
}

Transition* TransitionManager::CreateFadeToColour(float step_duration, Color fade_colour)
{	
	if (step == TransitionStep::NONE)
	{
		currentTransition = new FadeToColour(step_duration, fade_colour);
	}

	return currentTransition;
}

Transition* TransitionManager::CreateSlide(float step_duration, bool non_lerp, bool vertical, bool slide_from_right, bool slide_from_bottom, Color slide_colour)
{
	if (step == TransitionStep::NONE)
	{
		currentTransition = new Slide(step_duration, non_lerp, vertical, slide_from_right, slide_from_bottom, slide_colour);
	}

	return currentTransition;
}

Transition* TransitionManager::CreateWipe(float step_duration, bool non_lerp, bool vertical, bool wipe_from_right, bool wipe_from_bottom, Color fade_colour)
{
	if (step == TransitionStep::NONE)
	{
		currentTransition = new Wipe(step_duration, non_lerp, vertical, wipe_from_right, wipe_from_bottom, fade_colour);
	}

	return currentTransition;
}

Transition* TransitionManager::CreateAlternatingBars(float step_duration, bool non_lerp, int bar_number, bool vertical, bool random_colours, Color even_color, Color odd_color)
{
	if (step == TransitionStep::NONE)
	{
		currentTransition = new AlternatingBars(step_duration, non_lerp, bar_number, vertical, random_colours, even_color, odd_color);
	}

	return currentTransition;
}

Transition* TransitionManager::CreateExpandingBars(float step_duration, bool non_lerp, int bar_number, bool vertical, bool random_colours, Color even_color, Color odd_color)
{
	if (step == TransitionStep::NONE)
	{
		currentTransition = new ExpandingBars(step_duration, non_lerp, bar_number, vertical, random_colours, even_color, odd_color);
	}

	return currentTransition;
}

Transition* TransitionManager::CreateZoomToTexture(iPoint mouse_position, float step_duration, float zoom_scale)
{
	if (step == TransitionStep::NONE)
	{
		currentTransition = new ZoomToTexture(mouse_position, step_duration, zoom_scale);
	}

	return currentTransition;
}

Transition* TransitionManager::CreateDissolve(float step_duration)
{
	if (step == TransitionStep::NONE)
	{
		currentTransition = new Dissolve(step_duration);
	}

	return currentTransition;
}