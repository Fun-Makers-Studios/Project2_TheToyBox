#ifndef __SCENEENDING_H__
#define __SCENEENDING_H__

#include "Scene.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class SceneEnding : public Scene
{
public:

	SceneEnding();
	virtual ~SceneEnding();

	bool Awake(pugi::xml_node& config);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

public:
	uint startSFX = 0;

private:
	SDL_Texture* img = nullptr;

	const char* imgPath;
	const char* musicPath;
	const char* startSFXPath;

};


#endif // !__SCENEENDING_H__