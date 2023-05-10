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
	uint titleSFX = 0;
	uint startSFX = 0;
	uint menuSelectionSFX = 0;
	uint selectSFX = 0;
	uint closemenuSFX = 0;

	// UI
	GuiButton* continueButton = nullptr; // Show only if there is a saved game in "save_game.xml"
	GuiButton* playButton = nullptr;
	GuiButton* settingsButton = nullptr;
	GuiButton* creditsButton = nullptr;
	GuiButton* exitButton = nullptr;

	enum class ButtonID
	{
		CONTINUE,
		PLAY,
		SETTINGS,
		CREDITS,
		EXIT,
	};

private:
	SDL_Texture* img = nullptr;
	SDL_Texture* popImg_settings = nullptr;
	SDL_Texture* popImg_credits = nullptr;

	const char* imgPath;
	const char* popImgSettingsPath;
	const char* popImgCreditsPath;
	const char* musicPath;
	const char* startSFXPath;
	const char* selectSFXPath;
	const char* select2SFXPath;

	bool isSaved = false;

	bool exitGame = false;

};

#endif // __MENU_TITLE_H__