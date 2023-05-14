#include "FadeToColour.h"
#include "TransitionManager.h"

FadeToColour::FadeToColour(float step_duration, Color fade_colour) : Transition(step_duration), fade_colour(fade_colour)
{	
	InitFadeToColour();
}

FadeToColour::~FadeToColour() {}

void FadeToColour::DoTransition()
{
	SDL_SetRenderDrawColor(app->render->renderer, fade_colour.r, fade_colour.g, fade_colour.b, current_cutoff * 255.0f);

	SDL_RenderFillRect(app->render->renderer, &screen);
}

void FadeToColour::InitFadeToColour()
{
	app->win->GetWindowRect(screen);

	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);

	app->transitionManager->step = TransitionStep::IN;
}