#ifndef __MENU_SETTINGS_H__
#define __MENU_SETTINGS_H__

#include "Menu.h"
#include "GuiButton.h"

struct SDL_Texture;

class MenuSettings : public Menu
{
public:

	MenuSettings();
	virtual ~MenuSettings();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

public:

	uint startSFX = 0;
	uint menuSelectionSFX = 0;
	uint selectSFX = 0;
	uint closemenuSFX = 0;

	// UI
	GuiButton* decreaseMusicButton8 = nullptr;
	GuiButton* increaseMusicButton9 = nullptr;
	GuiButton* decreaseSFXButton10 = nullptr;
	GuiButton* increaseSFXButton11 = nullptr;
	GuiButton* fullscreenButton12 = nullptr;
	GuiButton* vsyncButton13 = nullptr;


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

#endif // __MENU_SETTINGS_H__
