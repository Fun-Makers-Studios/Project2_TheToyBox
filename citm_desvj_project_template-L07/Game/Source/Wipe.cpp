#include "Wipe.h"
#include "TransitionManager.h"

Wipe::Wipe(TransitionStart transitionStart, Color wipe_colour)
{
	this->transitionStart = transitionStart;
	this->wipe_colour = wipe_colour;

	InitWipe();
}

Wipe::~Wipe() {}

void Wipe::InitWipe()
{
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);

	app->win->GetWindowRect(screen);
}

void Wipe::DoTransition()
{
	switch (transitionStart)
	{
	case TransitionStart::TOP:

		if (app->transitionManager->step == TransitionStep::IN)
		{
			screen.y = app->transitionManager->easing->EasingAnimation(-screen.h, 0, app->transitionManager->trackedTime, app->transitionManager->easingTypeIN);
		}
		else if (app->transitionManager->step == TransitionStep::OUT)
		{
			screen.y = app->transitionManager->easing->EasingAnimation(0, screen.h, app->transitionManager->trackedTime, app->transitionManager->easingTypeOUT);
		}

		break;

	case TransitionStart::BOT:

		if (app->transitionManager->step == TransitionStep::IN)
		{
			screen.y = app->transitionManager->easing->EasingAnimation(screen.h, 0, app->transitionManager->trackedTime, app->transitionManager->easingTypeIN);
		}
		else if (app->transitionManager->step == TransitionStep::OUT)
		{
			screen.y = app->transitionManager->easing->EasingAnimation(0, -screen.h, app->transitionManager->trackedTime, app->transitionManager->easingTypeOUT);
		}

		break;

	case TransitionStart::LEFT:

		if (app->transitionManager->step == TransitionStep::IN)
		{
			screen.x = app->transitionManager->easing->EasingAnimation(-screen.w, 0, app->transitionManager->trackedTime, app->transitionManager->easingTypeIN);
		}
		else if (app->transitionManager->step == TransitionStep::OUT)
		{
			screen.x = app->transitionManager->easing->EasingAnimation(0, screen.w, app->transitionManager->trackedTime, app->transitionManager->easingTypeOUT);
		}

		break;

	case TransitionStart::RIGHT:

		if (app->transitionManager->step == TransitionStep::IN)
		{
			screen.x = app->transitionManager->easing->EasingAnimation(screen.w, 0, app->transitionManager->trackedTime, app->transitionManager->easingTypeIN);
		}
		else if (app->transitionManager->step == TransitionStep::OUT)
		{
			screen.x = app->transitionManager->easing->EasingAnimation(0, -screen.w, app->transitionManager->trackedTime, app->transitionManager->easingTypeOUT);
		}

		break;

	default:
		break;
	}

	DrawWipe();
}


void Wipe::DrawWipe()
{
	SDL_SetRenderDrawColor(app->render->renderer, wipe_colour.r, wipe_colour.g, wipe_colour.b, 255);
	SDL_RenderFillRect(app->render->renderer, &screen);
}