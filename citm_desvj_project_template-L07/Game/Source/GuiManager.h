#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "Scene.h"
#include "GuiControl.h"

#include "List.h"



class GuiManager : public Module
{
public:

	// Constructor
	GuiManager();

	// Destructor
	virtual ~GuiManager();

	// Called before the first frame
	 bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool Draw();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, int id, const char* text, int textSize, SDL_Rect bounds, Scene* observer, ButtonType buttonType = ButtonType::BIG, SDL_Rect sliderBounds = { 0,0,0,0 });
	void DestroyGuiControl(GuiControl* gui);
	//void AddGuiControl(GuiControl* gui);

public:

	List<GuiControl*> guiControlsList;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;

	SDL_Texture* texture;

	ButtonType buttonType;

};

#endif // __GUIMANAGER_H__
