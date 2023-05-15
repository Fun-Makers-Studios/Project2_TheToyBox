#include "AlternatingBars.h"
#include "TransitionManager.h"

AlternatingBars::AlternatingBars(int bar_number, bool vertical, bool random_colours, Color even_color, Color odd_color)
{
	this->bar_number = bar_number;
	this->win_width = 0.0f;
	this->win_height = 0.0f;
	this->vertical = vertical;
	this->random_colours = random_colours;
	this->even_color = even_color;
	this->odd_color = odd_color;
	
	InitAlternatingBars();
}

AlternatingBars::~AlternatingBars()
{
	bars.Clear();
}

void AlternatingBars::DoTransition()
{
	if (vertical)
	{
		TranslateVerticalBars();		
	}
	else
	{
		TranslateHorizontalBars();
	}

	DrawBars();
}

void AlternatingBars::TranslateHorizontalBars()
{
	if (app->transitionManager->step == TransitionStep::IN)
	{
		for (int i = 0; i < bars.Count(); ++i)
		{
			if (i % 2 == 0)
			{
				bars[i].bar.x = app->transitionManager->easing->EasingAnimation(-win_width, 0, app->transitionManager->trackedTime, app->transitionManager->easingTypeIN);
			}
			else
			{
				bars[i].bar.x = app->transitionManager->easing->EasingAnimation(win_width, 0, app->transitionManager->trackedTime, app->transitionManager->easingTypeIN);
			}
		}
	}
	else if (app->transitionManager->step == TransitionStep::OUT)
	{
		for (int i = 0; i < bars.Count(); ++i)
		{
			if (i % 2 == 0)
			{
				bars[i].bar.x = app->transitionManager->easing->EasingAnimation(0, win_width, app->transitionManager->trackedTime, app->transitionManager->easingTypeOUT);
			}
			else
			{
				bars[i].bar.x = app->transitionManager->easing->EasingAnimation(0, -win_width, app->transitionManager->trackedTime, app->transitionManager->easingTypeOUT);
			}
		}
	}
}

void AlternatingBars::TranslateVerticalBars()
{	
	if (app->transitionManager->step == TransitionStep::IN)
	{		
		for (int i = 0; i < bars.Count(); ++i)
		{
			if (i % 2 == 0)
			{
				bars[i].bar.y = app->transitionManager->easing->EasingAnimation(-win_height, 0, app->transitionManager->trackedTime, app->transitionManager->easingTypeIN);
			}
			else
			{
				bars[i].bar.y = app->transitionManager->easing->EasingAnimation(win_height, 0, app->transitionManager->trackedTime, app->transitionManager->easingTypeIN);
			}
		}
	}
	else if (app->transitionManager->step == TransitionStep::OUT)
	{
		for (int i = 0; i < bars.Count(); ++i)
		{
			if (i % 2 == 0)
			{
				bars[i].bar.y = app->transitionManager->easing->EasingAnimation(0, win_height, app->transitionManager->trackedTime, app->transitionManager->easingTypeOUT);
			}
			else
			{
				bars[i].bar.y = app->transitionManager->easing->EasingAnimation(0, -win_height, app->transitionManager->trackedTime, app->transitionManager->easingTypeOUT);
			}
		}
	}
}

void AlternatingBars::DrawBars()
{
	for (int i = 0; i < bars.Count(); ++i)
	{
		SDL_SetRenderDrawColor(app->render->renderer, bars[i].colour.r, bars[i].colour.g, bars[i].colour.b, 255);
		SDL_RenderFillRect(app->render->renderer, &bars[i].bar);
	}
}

void AlternatingBars::InitAlternatingBars()
{	
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	
	uint height, width;
	app->win->GetWindowSize(width, height);

	win_width = width;
	win_height = height;

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

	app->transitionManager->step = TransitionStep::IN;
}

void AlternatingBars::AssignHorizontalBar(Bar& new_bar, const int& win_width, const int& win_height, const int& index)
{
	int bar_width = win_width;
	
	float bar_num		= bar_number;																// As bar_number is an integer, when win_height is divided by it
 	float height_ratio	= win_height / bar_num;														// the decimals are cut off. By creating 2 float buffers and using
	int bar_height		= ceil(height_ratio);														// the ceil() function, an approximation is made to the real number.
	
	if (index % 2 == 0)
	{
		new_bar.bar = { -win_width, bar_height * index, bar_width, bar_height };
	}
	else
	{
		new_bar.bar = { win_width, bar_height * index, bar_width, bar_height };
	}
}

void AlternatingBars::AssignVerticalBar(Bar& new_bar, const int& win_width, const int& win_height, const int& index)
{
	int bar_height = win_height;

	float bar_num = bar_number;																		// As bar_number is an integer, when win_width is divided by it
	float width_ratio = win_width / bar_num;														// the decimals are cut off. By creating 2 float buffers and using
	int bar_width = ceil(width_ratio);																// the ceil() function, an approximation is made to the real number.

	if (index % 2 == 0)
	{
		new_bar.bar = { bar_width * index, -win_height, bar_width, bar_height };
	}
	else
	{
		new_bar.bar = { bar_width * index, win_height, bar_width, bar_height };
	}
}

void AlternatingBars::AssignBarColour(Bar& new_bar, const int& index)
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