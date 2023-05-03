#ifndef __SCENELOGO_H__
#define __SCENELOGO_H__

#include "Scene.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class SceneLogo : public Scene
{
public:

	SceneLogo();

	// Destructor
	virtual ~SceneLogo();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:

private:
	SDL_Texture* img = nullptr;
	int time = 0;

	const char* imgPath;
	const char* musicPath;

	//FX
	uint logoFX;

	// A frame count system to handle the fade time and ratio
	Uint32 frameCount = 250;
	Uint32 maxFadeFrames = 250;
	float fadeRatio;
};


#endif // !__SCENELOGO_H__

