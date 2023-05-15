#include "Dissolve.h"
#include "TransitionManager.h"

Dissolve::Dissolve(float step_duration)
{
	this->dissolving_alpha = 0.0f;
	this->condensing_alpha = 0.0f;

	InitDissolve();
}

Dissolve::~Dissolve() {}

void Dissolve::DoTransition()
{
	//dissolving_alpha += Lerp(MAX_ALPHA, MIN_ALPHA, dissolve_rate);	// Decreasing alpha value.
	//condensing_alpha += Lerp(MIN_ALPHA, MAX_ALPHA, dissolve_rate);	// Increasing alpha value.
	//
	//SDL_SetTextureAlphaMod(app->sceneManager->currentScene->scene_texture, dissolving_alpha);
	//SDL_SetTextureAlphaMod(app->sceneManager->currentScene->scene_texture, condensing_alpha);
}

void Dissolve::InitDissolve()
{
	/*dissolve_rate = GetCutoffRate(step_duration);
	
	app->sceneManager->LoadScene(next_scene);
	
	SDL_SetTextureAlphaMod(app->sceneManager->next_scene->tileset_texture, 0.0f);*/
}