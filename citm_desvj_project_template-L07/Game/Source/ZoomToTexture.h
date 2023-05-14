#ifndef __ZOOM_TO_TEXTURE_H__
#define __ZOOM_TO_TEXTURE_H__

#include  "Transition.h"

class ZoomToTexture : public Transition
{
public:

	ZoomToTexture(iPoint mouse_position, float step_duration, float zoom_scale);
	~ZoomToTexture();

	void DoTransition();

	void InitZoomToTexture(iPoint mouse_position);

private:

	iPoint mouse_position;
	
	float original_scale;
	float zoom_scale;
	float zoom_rate;

	float x_increase_rate;
	float y_increase_rate;

	SDL_Texture* target_to_zoom;

};

#endif // !__ZOOM_TO_TEXTURE_H__
