#include "Cut.h"
#include "TransitionManager.h"

Cut::Cut(SceneID next_scene) : Transition(next_scene, 0.0f)
{

}

Cut::~Cut()
{

}

void Cut::StepTransition()
{
	app->sceneManager->SwitchTo(next_scene);

	app->transitionManager->DeleteActiveTransition();
}