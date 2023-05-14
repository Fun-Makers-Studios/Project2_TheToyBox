#include "Slide.h"
#include "TransitionManager.h"

Slide::Slide(float step_duration, bool non_lerp, bool vertical, bool slide_from_right, bool slide_from_bottom, Color slide_colour)
	: Transition(step_duration, non_lerp)
	, vertical(vertical)
	, slide_from_right(slide_from_right)
	, slide_from_bottom(slide_from_bottom)
	, slide_colour(slide_colour)
{
	InitSlide();
}

Slide::~Slide()
{

}

void Slide::DoTransition()
{
	if (!vertical)
	{
		HorizontalSlide();
	}
	else
	{
		VerticalSlide();
	}

	DrawSlide();
}

void Slide::HorizontalSlide()
{
	if (!slide_from_right)
	{
		if (!non_lerp)
		{
			screen.x = Lerp(-screen.w, 0, current_cutoff);									// Horizontal linearly interpolated slide coming from the left.
		}
		else
		{
			//screen.x = N_Lerp(-screen.w, 0, current_cutoff, true);
			screen.x = N_Lerp(-screen.w, 0, current_cutoff);								// Horizontal non-linearly interpolated slide coming from the left.
		}
	}
	else
	{
		if (!non_lerp)
		{
			screen.x = Lerp(screen.w, 0, current_cutoff);									// Horizontal linearly interpolated slide coming from the right.
		}
		else
		{
			//screen.x = N_Lerp(screen.w, 0, current_cutoff, true);
			screen.x = N_Lerp(screen.w, 0, current_cutoff);									// Horizontal non-linearly interpolated slide coming from the right.
		}
	}
}

void Slide::VerticalSlide()
{
	if (!slide_from_bottom)
	{
		if (!non_lerp)
		{
			screen.y = Lerp(-screen.h, 0, current_cutoff);									// Vertical linearly interpolated slide coming from the top.
		}
		else
		{
			//screen.y = N_Lerp(-screen.h, 0, current_cutoff, true);
			screen.y = N_Lerp(-screen.h, 0, current_cutoff);								// Vertical non-linearly interpolated slide coming from the top.
		}
	}
	else
	{
		if (!non_lerp)
		{
			screen.y = Lerp(screen.h, 0, current_cutoff);									// Vertical linearly interpolated slide coming from the bottom.
		}
		else
		{
			//screen.y = N_Lerp(screen.h, 0, current_cutoff, true);
			screen.y = N_Lerp(screen.h, 0, current_cutoff);									// Vertical non-linearly interpolated slide coming from the bottom.
		}
	}
}

void Slide::DrawSlide()
{
	SDL_SetRenderDrawColor(app->render->renderer, slide_colour.r, slide_colour.g, slide_colour.b, 255);
	SDL_RenderFillRect(app->render->renderer, &screen);
}

void Slide::InitSlide()
{
	app->win->GetWindowRect(screen);

	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);

	app->transitionManager->step = TransitionStep::IN;
}