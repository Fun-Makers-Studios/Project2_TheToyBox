#include "TransitionManager.h"
#include "FadeToColor.h"
#include "Slide.h"
#include "Wipe.h"
#include "AlternatingBars.h"
#include "ExpandingBars.h"
#include "ZoomToTexture.h"
#include "Dissolve.h"
#include "Log.h"

TransitionManager::TransitionManager() : currentTransition(nullptr)
{
	easing = new Easing();
	easing->SetFinished(false);
	name.Create("transitionmanager");
}

TransitionManager::~TransitionManager()
{
	delete easing;
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

			trackedTime = easing->TrackTime(app->GetDT());

			if (easing->GetFinished())
			{
				easing->SetFinished(false);
				app->transitionManager->step = TransitionStep::SWITCH;
			}

			break;

		case TransitionStep::SWITCH:
			app->transitionManager->step = TransitionStep::OUT;

		case TransitionStep::OUT:

			trackedTime = easing->TrackTime(app->GetDT());

			if (easing->GetFinished())
			{
				easing->SetFinished(false);
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

void TransitionManager::LoadTransition()
{
	switch (app->sceneManager->nextScene)
	{
		case SceneID::SCENE_LOGO:

			break;

		case SceneID::SCENE_TITLE:
			CreateFadeToColour(Black);
			easing->SetTotalTime(1.5);
			easingTypeIN = EasingType::EASE_OUT_SIN;
			easingTypeOUT = EasingType::EASE_IN_SIN;
			break;

		case SceneID::SCENE_GAME:
			CreateWipe(TransitionStart::LEFT, Black);
			easing->SetTotalTime(1);
			easingTypeIN = EasingType::EASE_INOUT_SIN;
			easingTypeOUT = EasingType::EASE_INOUT_SIN;
			break;
		
		case SceneID::SCENE_CIRCUS:
			CreateWipe(TransitionStart::LEFT, Black);
			easing->SetTotalTime(1);
			easingTypeIN = EasingType::EASE_INOUT_SIN;
			easingTypeOUT = EasingType::EASE_INOUT_SIN;
			break;

		case SceneID::SCENE_FIGHT:
			CreateAlternatingBars(8, false, false, Black, Black);
			easing->SetTotalTime(2);
			easingTypeIN = EasingType::EASE_OUT_ELASTIC;
			easingTypeOUT = EasingType::EASE_OUT_CUBIC;
			break;

		case SceneID::SCENE_ENDING:
			break;

		default:
			break;
	}

	if (currentTransition != nullptr)
	{
		app->transitionManager->step = TransitionStep::IN;
	}
}

void TransitionManager::DeleteActiveTransition()
{
	delete currentTransition;
	currentTransition = nullptr;
}

Transition* TransitionManager::CreateFadeToColour(Color fade_color)
{	
	if (step == TransitionStep::NONE)
	{
		currentTransition = new FadeToColor(fade_color);
	}

	return currentTransition;
}

Transition* TransitionManager::CreateSlide(TransitionStart transitionStart, Color slide_colour)
{
	if (step == TransitionStep::NONE)
	{
		currentTransition = new Slide(transitionStart, slide_colour);
	}

	return currentTransition;
}

Transition* TransitionManager::CreateWipe(TransitionStart transitionStart, Color fade_color)
{
	if (step == TransitionStep::NONE)
	{
		currentTransition = new Wipe(transitionStart, fade_color);
	}

	return currentTransition;
}

Transition* TransitionManager::CreateAlternatingBars(int bar_number, bool vertical, bool random_colours, Color even_color, Color odd_color)
{
	if (step == TransitionStep::NONE)
	{
		currentTransition = new AlternatingBars(bar_number, vertical, random_colours, even_color, odd_color);
	}

	return currentTransition;
}

Transition* TransitionManager::CreateExpandingBars(int bar_number, bool vertical, bool random_colours, Color even_color, Color odd_color)
{
	if (step == TransitionStep::NONE)
	{
		currentTransition = new ExpandingBars(bar_number, vertical, random_colours, even_color, odd_color);
	}

	return currentTransition;
}

Transition* TransitionManager::CreateZoomToTexture(iPoint mouse_position, float zoom_scale)
{
	if (step == TransitionStep::NONE)
	{
		currentTransition = new ZoomToTexture(mouse_position, zoom_scale);
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