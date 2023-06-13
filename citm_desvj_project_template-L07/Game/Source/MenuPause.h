#ifndef __MENU_PAUSE_H__
#define __MENU_PAUSE_H__

#include "Menu.h"
#include "GuiControl.h"
#include "GuiButton.h"

struct SDL_Texture;

class MenuPause : public Menu
{
public:

	MenuPause();
	virtual ~MenuPause();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

public:

	// UI
	GuiButton* resumeButton = nullptr;
	GuiButton* backToTitleButton = nullptr;
	GuiButton* settingsButton = nullptr;
	GuiButton* saveExitButton = nullptr;

	enum class ControlID : uint32
	{
		RESUME,
		BACK_TO_TITLE,
		SETTINGS,
		SAVE_EXIT
	};

private:

	SDL_Texture* imgPause = nullptr;
	const char* imgPausePath = nullptr;
	const char* music;

	SDL_Rect pauseRect;
};

#endif // __MENU_PAUSE_H__