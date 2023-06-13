#include "ZoomToTexture.h"
#include "TransitionManager.h"
#include "Map.h"

ZoomToTexture::ZoomToTexture(iPoint mouse_position, float zoom_scale)
{
	this->zoom_scale = zoom_scale;
	this->mouse_position = mouse_position;

	InitZoomToTexture(mouse_position);
}

ZoomToTexture::~ZoomToTexture() {}

void ZoomToTexture::DoTransition()
{
	//zoom_rate = original_scale + current_cutoff * zoom_scale;															// If current_cutoff == 0.0f, then scale == original_scale.

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
	//step = TransitionStep::IN;
}