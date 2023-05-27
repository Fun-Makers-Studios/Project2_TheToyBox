#ifndef __SCENELOGO_H__
#define __SCENELOGO_H__

#include "Scene.h"
#include "Easing.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class SceneLogo : public Scene
{
public:

	SceneLogo();
	virtual ~SceneLogo();

	bool Awake(pugi::xml_node& config);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

private:

	SDL_Texture* img = nullptr;
	int time = 0;

	const char* imgPath;
	const char* musicPath;

	//FX
	uint logoFX;

	Easing* easingLogo;
};


#endif // !__SCENELOGO_H__