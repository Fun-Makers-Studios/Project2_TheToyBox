#include "ExpandingBars.h"
#include "TransitionManager.h"

ExpandingBars::ExpandingBars(SceneID next_scene, float step_duration, bool non_lerp, int bar_number, bool vertical, bool random_colours, Color even_color, Color odd_color) 
	: Transition(next_scene, step_duration, non_lerp)
	, bar_number(bar_number)
	, random_colours(random_colours)
	, even_color(even_color)
	, odd_color(odd_color)
	, vertical(vertical)
{
	InitExpandingBars();
}

ExpandingBars::~ExpandingBars()
{

}

void ExpandingBars::StepTransition()
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

	ExpandBars();
}

void ExpandingBars::Entering()
{
	current_cutoff += GetCutoffRate(step_duration);

	if (current_cutoff >= MAX_CUTOFF)
	{
		current_cutoff = MAX_CUTOFF;
		
		step = TransitionStep::CHANGING;
	}
}

void ExpandingBars::Changing()
{
	app->sceneManager->SwitchTo(next_scene);

	step = TransitionStep::EXITING;
}

void ExpandingBars::Exiting()
{
	current_cutoff -= GetCutoffRate(step_duration);
	
	if (current_cutoff <= MIN_CUTOFF)
	{
		step = TransitionStep::NONE;

		bars.Clear();

		app->transitionManager->DeleteActiveTransition();
	}
}

void ExpandingBars::ExpandBars()
{
	if (!vertical)
	{
		ExpandHorizontalBars();
	}
	else
	{
		ExpandVerticalBars();
	}

	DrawBars();
}

void ExpandingBars::ExpandHorizontalBars()
{
	if (step == TransitionStep::ENTERING)
	{
		if (!non_lerp)
		{
			for (int i = 0; i < bars.Count(); ++i)												// cutoff goes from 0 to 1 to 0, so there would be no need to
			{																					// separate expansion and reduction in different steps.
				bars[i].bar.x = Lerp(screen_center.x, 0, current_cutoff);
				bars[i].bar.w = Lerp(0, win_width, current_cutoff);
			}
		}
		else
		{
			for (int i = 0; i < bars.Count(); ++i)												// cutoff goes from 0 to 1 to 0, so there would be no need to
			{																					// separate expansion and reduction in different steps.
				if (i % 2 == 0)																	// Ex: 0 --> win_width --> 0.
				{
					bars[i].bar.x = N_Lerp(screen_center.x, 0, current_cutoff);
					bars[i].bar.w = N_Lerp(0, win_width, current_cutoff);
				}
				else
				{
					bars[i].bar.x = N_Lerp(screen_center.x, 0, current_cutoff, true);
					bars[i].bar.w = N_Lerp(0, win_width, current_cutoff, true);
				}
			}
		}
	}

	if (step == TransitionStep::EXITING)
	{
		for (int i = 0; i < bars.Count(); ++i)												// As in the exiting step all bars will be at the same position/size, we need Lerp().
		{																					// By using Lerp(), non_lerp and even/odd bar distinction is not needed.
			bars[i].bar.x = Lerp(screen_center.x, 0, current_cutoff);
			bars[i].bar.w = Lerp(0, win_width, current_cutoff);
		}
	}
}

void ExpandingBars::ExpandVerticalBars()
{
	if (step == TransitionStep::ENTERING)
	{
		if (!non_lerp)
		{
			for (int i = 0; i < bars.Count(); ++i)
			{
				bars[i].bar.y = Lerp(screen_center.y, 0, current_cutoff);
				bars[i].bar.h = Lerp(0, win_height, current_cutoff);
			}
		}
		else
		{
			for (int i = 0; i < bars.Count(); ++i)												// cutoff goes from 0 to 1 to 0, so there would be no need to
			{																					// separate expansion and reduction in different steps.
				if (i % 2 == 0)																	// Ex: 0 --> win_width --> 0.
				{
					bars[i].bar.y = N_Lerp(screen_center.y, 0, current_cutoff);
					bars[i].bar.h = N_Lerp(0, win_height, current_cutoff);
				}
				else
				{
					bars[i].bar.y = N_Lerp(screen_center.y, 0, current_cutoff, true);
					bars[i].bar.h = N_Lerp(0, win_height, current_cutoff, true);
				}
			}
		}
	}

	if (step == TransitionStep::EXITING)
	{
		for (int i = 0; i < bars.Count(); ++i)													// cutoff goes from 0 to 1 to 0, so there would be no need to									
		{																						// separate expansion and reduction in different steps.
			bars[i].bar.y = Lerp(screen_center.y, 0, current_cutoff);							// Ex: 0 --> win_height --> 0.
			bars[i].bar.h = Lerp(0, win_height, current_cutoff);
		}
	}
}

void ExpandingBars::DrawBars()
{
	for (int i = 0; i < bars.Count(); ++i)
	{
		SDL_SetRenderDrawColor(app->render->renderer, bars[i].colour.r, bars[i].colour.g, bars[i].colour.b, 255);
		SDL_RenderFillRect(app->render->renderer, &bars[i].bar);
	}
}

void ExpandingBars::InitExpandingBars()
{
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	
	uint height, width;
	app->win->GetWindowSize(width, height);

	win_width = width;
	win_height = height;

	screen_center = { win_width * 0.5f, win_height * 0.5f };

	for (int i = 0; i < bar_number; ++i)
	{
		Bar new_bar;

		if (!vertical)
		{
			AssignHorizontalBar(new_bar, win_width, win_height, i);
		}
		else
		{
			AssignVerticalBar(new_bar, win_width, win_height, i);
		}
		
		AssignBarColour(new_bar, i);

		bars.Add(new_bar);
	}

	step = TransitionStep::ENTERING;
}

void ExpandingBars::AssignHorizontalBar(Bar& new_bar, const int& win_width, const int& win_height, const int& index)
{
	int bar_width = win_width;

	float bar_num = bar_number;																		// As bar_number is an integer, when win_height is divided by it
	float height_ratio = win_height / bar_num;														// the decimals are cut off. By creating 2 float buffers and using
	int bar_height = ceil(height_ratio);															// the ceil() function, an approximation to the real number is made.

	new_bar.bar = { (int)screen_center.x, bar_height * index, 0, bar_height };
}

void ExpandingBars::AssignVerticalBar(Bar& new_bar, const int& win_width, const int& win_height, const int& index)
{
	int bar_height = win_height;
	
	float bar_num = bar_number;																		// As bar_number is an integer, when win_width is divided by it
	float width_ratio = win_width / bar_num;														// the decimals are cut off. By creating 2 float buffers and using
	int bar_width = ceil(width_ratio);																// the ceil() function, an approximation is made to the real number.

	new_bar.bar = { bar_width * index, (int)screen_center.y, bar_width, 0 };
}

void ExpandingBars::AssignBarColour(Bar& new_bar, const int& index)
{
	if (random_colours)
	{
		new_bar.colour = Blue/*GetRandomColour()*/;
	}
	else
	{
		if (index % 2 == 0)
		{
			new_bar.colour = even_color;
		}
		else
		{
			new_bar.colour = odd_color;
		}
	}
}