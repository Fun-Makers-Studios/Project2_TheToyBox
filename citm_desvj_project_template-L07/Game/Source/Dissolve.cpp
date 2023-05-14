#include "Dissolve.h"
#include "TransitionManager.h"

Dissolve::Dissolve(SceneID next_scene, float step_duration) : Transition(next_scene, step_duration)
, dissolving_alpha(0.0f)
, condensing_alpha(0.0f)
{
	InitDissolve();
}

Dissolve::~Dissolve()
{

}

void Dissolve::StepTransition()
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

	ApplyDissolve();
}

void Dissolve::Entering()
{
	current_cutoff += GetCutoffRate(step_duration);

	if (current_cutoff <= MAX_CUTOFF)
	{
		current_cutoff = MAX_CUTOFF;

		step = TransitionStep::CHANGING;
	}
}

void Dissolve::Changing()
{
	//app->sceneManager->UnloadScene(app->sceneManager->current_scene);

	step = TransitionStep::EXITING;
}

void Dissolve::Exiting()
{
	step = TransitionStep::NONE;
	
	app->transitionManager->DeleteActiveTransition();
}

void Dissolve::ApplyDissolve()
{
	dissolving_alpha += Lerp(MAX_ALPHA, MIN_ALPHA, dissolve_rate);									// Decreasing alpha value.
	condensing_alpha += Lerp(MIN_ALPHA, MAX_ALPHA, dissolve_rate);									// Increasing alpha value.
	
	//SDL_SetTextureAlphaMod(app->sceneManager->current_scene->scene_texture, dissolving_alpha);
	//SDL_SetTextureAlphaMod(app->sceneManager->next_scene->scene_texture, condensing_alpha);
}

void Dissolve::InitDissolve()
{
	/*dissolve_rate = GetCutoffRate(step_duration);
	
	app->sceneManager->LoadScene(next_scene);
	
	SDL_SetTextureAlphaMod(app->sceneManager->next_scene->tileset_texture, 0.0f);*/

	step = TransitionStep::ENTERING;
}