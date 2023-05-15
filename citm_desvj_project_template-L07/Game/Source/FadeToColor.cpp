#include "FadeToColor.h"
#include "TransitionManager.h"

FadeToColor::FadeToColor(Color fade_color)
{	
	this->fade_color = fade_color;

	InitFadeToColour();
}

FadeToColor::~FadeToColor() {}

void FadeToColor::DoTransition()
{
	if (app->transitionManager->step == TransitionStep::OUT)
	{
		uint easedAlpha = app->transitionManager->easing->EasingAnimation(255, 0, app->transitionManager->trackedTime, app->transitionManager->easingTypeOUT);
		SDL_SetRenderDrawColor(app->render->renderer, fade_color.r, fade_color.g, fade_color.b, easedAlpha);
	}
	else
	{
		uint easedAlpha = app->transitionManager->easing->EasingAnimation(0, 255, app->transitionManager->trackedTime, app->transitionManager->easingTypeIN);
		SDL_SetRenderDrawColor(app->render->renderer, fade_color.r, fade_color.g, fade_color.b, easedAlpha);
	}

	SDL_RenderFillRect(app->render->renderer, &screen);
}

void FadeToColor::InitFadeToColour()
{
	app->win->GetWindowRect(screen);

	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
}