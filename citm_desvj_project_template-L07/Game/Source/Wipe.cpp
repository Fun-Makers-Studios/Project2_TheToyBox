#include "Wipe.h"
#include "TransitionManager.h"

Wipe::Wipe(float step_duration, bool non_lerp, bool vertical, bool wipe_from_right, bool wipe_from_bottom, Color wipe_colour) : Transition(step_duration, non_lerp),
vertical(vertical),
wipe_from_right(wipe_from_right),
wipe_from_bottom(wipe_from_bottom),
wipe_colour(wipe_colour)
{
	InitWipe();
}

Wipe::~Wipe() {}

void Wipe::DoTransition()
{
	if (!vertical)
	{
		HorizontalWipe();
	}
	else
	{
		VerticalWipe();
	}

	DrawWipe();
}

void Wipe::HorizontalWipe()
{
	if (!wipe_from_right)
	{
		if (app->transitionManager->step == TransitionStep::IN)												// Horizontal wipe that will start from (-screen.w, 0 ) and will stop at ( 0, 0 ).
		{
			if (!non_lerp)
			{
				screen.x = Lerp(-screen.w, 0, current_cutoff);								// Linearly interpolated wipe.
			}
			else
			{
				//screen.x = N_Lerp(-screen.w, 0, current_cutoff, true);
				screen.x = N_Lerp(-screen.w, 0, current_cutoff);							// Non-linearly interpolated wipe.
			}
		}

		if (app->transitionManager->step == TransitionStep::OUT)												// Horizontal wipe that will start from ( 0, 0 ) and will stop at ( sceen.w , 0 ).
		{
			if (!non_lerp)
			{
				screen.x = Lerp(0, screen.w, current_cutoff);
			}
			else
			{
				//screen.x = N_Lerp(0, screen.w, current_cutoff, true);
				screen.x = N_Lerp(0, screen.w, current_cutoff);
			}
		}
	}
	else
	{
		if (app->transitionManager->step == TransitionStep::IN)												// Horizontal wipe that will start from ( screen.w, 0 ) and will stop at ( 0, 0 ).
		{
			if (!non_lerp)
			{
				screen.x = Lerp(screen.w, 0, current_cutoff);
			}
			else
			{
				//screen.x = N_Lerp(screen.w, 0, current_cutoff, true);
				screen.x = N_Lerp(screen.w, 0, current_cutoff);
			}
		}

		if (app->transitionManager->step == TransitionStep::OUT)												// Horizontal wipe that will start from ( 0, 0 ) and will stop at ( -sceen.w , 0 ).
		{
			if (!non_lerp)
			{
				screen.x = Lerp(0, -screen.w, current_cutoff);
			}
			else
			{
				//screen.x = N_Lerp(0, -screen.w, current_cutoff, true);
				screen.x = N_Lerp(0, -screen.w, current_cutoff);
			}
		}
	}
}

void Wipe::VerticalWipe()
{
	if (!wipe_from_bottom)
	{
		if (app->transitionManager->step == TransitionStep::IN)												// Vertical wipe that will start from ( 0, screen.h ) and will stop at ( 0, 0 ).
		{
			if (!non_lerp)
			{
				screen.y = Lerp(-screen.h, 0, current_cutoff);								// Linearly interpolated wipe.
			}
			else
			{
				//screen.y = N_Lerp(-screen.h, 0, current_cutoff, true);
				screen.y = N_Lerp(-screen.h, 0, current_cutoff);							// Non-linearly interpolated wipe.
			}
		}

		if (app->transitionManager->step == TransitionStep::OUT)												// Vertical wipe that will start from ( 0, 0 ) and will stop at ( 0, screen.h ).
		{
			if (!non_lerp)
			{
				screen.y = Lerp(0, screen.h, current_cutoff);
			}
			else
			{
				//screen.y = N_Lerp(0, screen.h, current_cutoff, true);
				screen.y = N_Lerp(0, screen.h, current_cutoff);
			}
		}
	}
	else
	{
		if (app->transitionManager->step == TransitionStep::IN)												// Vertical wipe that will start from ( 0, screen.h ) and will stop at ( 0, 0 ).
		{
			if (!non_lerp)
			{
				screen.y = Lerp(screen.h, 0, current_cutoff);
			}
			else
			{
				//screen.y = N_Lerp(screen.h, 0, current_cutoff, true);
				screen.y = N_Lerp(screen.h, 0, current_cutoff);
			}
		}

		if (app->transitionManager->step == TransitionStep::OUT)												// Vertical wipe that will start from ( 0, 0 ) and will stop at ( 0, -screen.h ).
		{
			if (!non_lerp)
			{
				screen.y = Lerp(0, -screen.h, current_cutoff);
			}
			else
			{
				//screen.x = N_Lerp(0, -screen.h, current_cutoff, true);
				screen.y = N_Lerp(0, -screen.h, current_cutoff);
			}
		}
	}
}

void Wipe::DrawWipe()
{
	SDL_SetRenderDrawColor(app->render->renderer, wipe_colour.r, wipe_colour.g, wipe_colour.b, 255);
	SDL_RenderFillRect(app->render->renderer, &screen);
}

void Wipe::InitWipe()
{
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);

	app->win->GetWindowRect(screen);

	app->transitionManager->step = TransitionStep::IN;
}