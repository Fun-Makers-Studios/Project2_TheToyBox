#include "ZoomToTexture.h"
#include "TransitionManager.h"
#include "Map.h"

ZoomToTexture::ZoomToTexture(SceneID next_scene, iPoint mouse_position, float step_duration, float zoom_scale) : Transition(next_scene, step_duration)
, zoom_scale(zoom_scale)
, mouse_position(mouse_position)
{
	InitZoomToTexture(mouse_position);
}

ZoomToTexture::~ZoomToTexture()
{

}

void ZoomToTexture::StepTransition()
{
	//current_cutoff += GetCutoffRate(step_duration);
	
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

	ApplyZoom();
}

void ZoomToTexture::Entering()
{
	current_cutoff += GetCutoffRate(step_duration);
	
	if (current_cutoff >= MAX_CUTOFF)
	{
		current_cutoff = MAX_CUTOFF;

		step = TransitionStep::CHANGING;
	}
}

void ZoomToTexture::Changing()
{
	app->sceneManager->SwitchTo(next_scene);

	//target_to_zoom = app->sceneManager->currentScene->scene_texture;

	step = TransitionStep::EXITING;
}

void ZoomToTexture::Exiting()
{
	current_cutoff -= GetCutoffRate(step_duration);
	
	if (current_cutoff <= MIN_CUTOFF)
	{	
		step = TransitionStep::NONE;

		app->transitionManager->DeleteActiveTransition();
	}
}

void ZoomToTexture::ApplyZoom()
{
	zoom_rate = original_scale + current_cutoff * zoom_scale;															// If current_cutoff == 0.0f, then scale == original_scale.

	//app->render->Blit(target_to_zoom, x_increase_rate, y_increase_rate, NULL, true, app->render->renderer, zoom_rate);
}

void ZoomToTexture::InitZoomToTexture(iPoint mouse_position)
{
	//int x_offset;
	//int y_offset;
	//
	//app->map->GetTileOffset(x_offset, y_offset);
	//
	//original_scale			= app->win->GetScale();																				// The minimum amount of zoom that there will be.	
	//
	//target_to_zoom			= app->sceneManager->current_scene->scene_texture;													// The texture to which scale will be incremented.
	//
	//float total_zoom		= original_scale + zoom_scale;																		// The maximum amount of zoom that there will be.
	//
	//float half_map_width	= app->sceneManager->currentScene->map_width * 0.5f;												// Will be used to keep up with the zoom's rate of increase.
	//float half_map_height	= app->sceneManager->currentScene->map_height * 0.5f;										
	//
	//float x_increment		= Lerp(half_map_width, half_map_width * zoom_scale, zoom_rate);
	//float y_increment		= Lerp(half_map_height, half_map_height * zoom_scale, zoom_rate);

	//x_increase_rate			= -mouse_position.x - x_increment + x_offset;
	//y_increase_rate			= -mouse_position.y + y_increment - y_offset;
	//
	//
	//step = TransitionStep::ENTERING;
}