#include "Slide.h"
#include "TransitionManager.h"

Slide::Slide(TransitionStart transitionStart, Color slide_colour)
{
	this->transitionStart = transitionStart;
	this->slide_colour = slide_colour;

	InitSlide();
}

Slide::~Slide()
{

}

void Slide::InitSlide()
{
	app->win->GetWindowRect(screen);

	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
}

void Slide::DoTransition()
{
	double time;

	if (app->transitionManager->step == TransitionStep::OUT)
	{
		time = 1 - app->transitionManager->trackedTime;
	}
	else
	{
		time = app->transitionManager->trackedTime;
	}

	switch (transitionStart)
	{
	case TransitionStart::TOP:

		screen.y = app->transitionManager->easing->EasingAnimation(-screen.h, 0, time, app->transitionManager->easingTypeOUT);

		break;

	case TransitionStart::BOT:

		screen.y = app->transitionManager->easing->EasingAnimation(screen.h, 0, time, app->transitionManager->easingTypeOUT);

		break;

	case TransitionStart::LEFT:

		screen.x = app->transitionManager->easing->EasingAnimation(-screen.w, 0, time, app->transitionManager->easingTypeOUT);

		break;

	case TransitionStart::RIGHT:

		screen.x = app->transitionManager->easing->EasingAnimation(screen.w, 0, time, app->transitionManager->easingTypeOUT);

		break;

	default:
		break;
	}

	DrawSlide();
}

void Slide::DrawSlide()
{
	SDL_SetRenderDrawColor(app->render->renderer, slide_colour.r, slide_colour.g, slide_colour.b, 255);
	SDL_RenderFillRect(app->render->renderer, &screen);
}
