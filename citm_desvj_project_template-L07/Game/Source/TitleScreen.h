#ifndef __TITLESCREEN_H__
#define __TITLESCREEN_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include "GuiButton.h"

struct SDL_Texture;

class TitleScreen : public Module
{
public:

	TitleScreen();

	// Destructor
	virtual ~TitleScreen();

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
	uint titleSFX = 0;
	uint startSFX = 0;
	uint menuSelectionSFX = 0;
	uint selectSFX = 0;
	uint closemenuSFX = 0;

	// Declare a GUI Button and create it using the GuiManager
	GuiButton* playButton1 = nullptr;
	GuiButton* settingsButton2 = nullptr;
	GuiButton* creditsButton3 = nullptr;
	GuiButton* exitButton4 = nullptr;
	GuiButton* continueButton5 = nullptr; // Show only if there is a saved game in "save_game.xml"
	

	GuiButton* decreaseMusicButton8 = nullptr;
	GuiButton* increaseMusicButton9 = nullptr;

	GuiButton* decreaseSFXButton10 = nullptr;
	GuiButton* increaseSFXButton11 = nullptr;

	GuiButton* fullscreenButton12 = nullptr;

	GuiButton* vsyncButton13 = nullptr;

	bool settingMenu = false;
	bool creditsMenu = false;

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

#endif // !__TITLESCREEN_H__

