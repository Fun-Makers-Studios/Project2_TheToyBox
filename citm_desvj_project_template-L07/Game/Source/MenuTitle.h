#ifndef __MENU_TITLE_H__
#define __MENU_TITLE_H__

#include "Menu.h"
#include "GuiControl.h"
#include "GuiButton.h"

struct SDL_Texture;

class MenuTitle : public Menu
{
public:

	MenuTitle();
	virtual ~MenuTitle();

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
	GuiButton* continueButton = nullptr; // Show only if there is a saved game in "save_game.xml"
	GuiButton* playButton = nullptr;
	GuiButton* settingsButton = nullptr;
	GuiButton* creditsButton = nullptr;
	GuiButton* exitButton = nullptr;

	enum class ControlID
	{
		CONTINUE,
		PLAY,
		SETTINGS,
		CREDITS,
		EXIT,
	};

private:

	bool isSaved = false;
	bool exitGame = false;

};

#endif // __MENU_TITLE_H__