#ifndef __TITLESCREEN_H__
#define __TITLESCREEN_H__

#include "Scene.h"
#include "SDL/include/SDL.h"
#include "GuiButton.h"

struct SDL_Texture;

class SceneTitle : public Scene
{
public:

	SceneTitle();

	// Destructor
	virtual ~SceneTitle();

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

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

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

};

#endif // __SCENE_TITLE_H__