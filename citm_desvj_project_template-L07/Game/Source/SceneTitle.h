#ifndef __TITLESCREEN_H__
#define __TITLESCREEN_H__

#include "Scene.h"
#include "SDL/include/SDL.h"
#include "GuiButton.h"

struct SDL_Texture;

enum class TilteEasingState
{
	UP,
	DOWN
};

class SceneTitle : public Scene
{
public:

	SceneTitle();
	virtual ~SceneTitle();

	bool Awake(pugi::xml_node& config);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

public:

	uint titleIntroSFX = 0;	

	bool settingMenu = false;
	bool creditsMenu = false;

private:

	SDL_Texture* img = nullptr;
	SDL_Texture* nameImg = nullptr;

	const char* imgPath;
	const char* nameImgPath;
	const char* musicPath;
	const char* select2SFXPath;

	bool isSaved = false;
	bool exitGame = false;

	Easing* easingTitle = nullptr;

	TilteEasingState tilteEasingState;
};

#endif // __SCENE_TITLE_H__