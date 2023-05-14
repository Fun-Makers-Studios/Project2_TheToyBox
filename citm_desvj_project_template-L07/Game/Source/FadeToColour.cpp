#include "FadeToColour.h"
#include "TransitionManager.h"

FadeToColour::FadeToColour(SceneID next_scene, float step_duration, Color fade_colour) : Transition(next_scene, step_duration)
, fade_colour(fade_colour)
{	
	InitFadeToColour();
}

FadeToColour::~FadeToColour()
{

}

void FadeToColour::StepTransition()
{
	switch (step)
	{
	case TransitionStep::ENTERING:
		
		Entering();
		
		break;

	case TransitionStep::CHANGING:

		Changing();

		break;

	case TransitionStep::EXITING:
		
		Exiting();
		
		break;
	}

	ApplyFade();
}

void FadeToColour::Entering()
{
	current_cutoff += GetCutoffRate(step_duration);

	if (current_cutoff >= MAX_CUTOFF)
	{
		current_cutoff = MAX_CUTOFF;

		step = TransitionStep::CHANGING;
	}
}

void FadeToColour::Changing()
{
	app->sceneManager->SwitchTo(next_scene);

	step = TransitionStep::EXITING;
}

void FadeToColour::Exiting()
{
	current_cutoff -= GetCutoffRate(step_duration);

	if (current_cutoff <= MIN_CUTOFF)
	{
		current_cutoff = MIN_CUTOFF;

		step = TransitionStep::NONE;

		app->transitionManager->DeleteActiveTransition();
	}
}

void FadeToColour::ApplyFade()
{
	SDL_SetRenderDrawColor(app->render->renderer, fade_colour.r, fade_colour.g, fade_colour.b, current_cutoff * 255.0f);

	SDL_RenderFillRect(app->render->renderer, &screen);
}

void FadeToColour::InitFadeToColour()
{
	app->win->GetWindowRect(screen);

	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);

	step = TransitionStep::ENTERING;
}