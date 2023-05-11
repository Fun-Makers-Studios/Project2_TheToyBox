#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__

#include "Scene.h"
#include "SDL/include/SDL.h"
#include "GuiButton.h"

struct SDL_Texture;

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

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

public:

	uint titleIntroSFX = 0;	

	bool settingMenu = false;
	bool creditsMenu = false;

private:
	SDL_Texture* img = nullptr;

	const char* imgPath;
	const char* musicPath;
	const char* select2SFXPath;

	bool isSaved = false;
	bool exitGame = false;

};

#endif // __SCENE_TITLE_H__