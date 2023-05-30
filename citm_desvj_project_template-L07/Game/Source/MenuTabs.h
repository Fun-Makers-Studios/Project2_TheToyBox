#ifndef __MENU_TABS_H__
#define __MENU_TABS_H__

#include "Menu.h"
#include "GuiControl.h"
#include "GuiButton.h"
#include "Animation.h"


struct SDL_Texture;

class MenuTabs : public Menu
{
public:

	MenuTabs();
	virtual ~MenuTabs();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

public:

	// Animation
	Animation* currentAnim = nullptr;
	Animation popOut;

	// UI
	GuiButton* partyButton = nullptr;
	GuiButton* questsButton = nullptr;
	GuiButton* savesButton = nullptr;
	GuiButton* settingsButton = nullptr;
	GuiButton* creditsButton = nullptr;

	enum class ControlID : uint32
	{
		PARTY,
		QUESTS,
		SAVES,
		SETTINGS,
		CREDITS
	};

private:

	SDL_Texture* imgTabs = nullptr;
	const char* imgTabsPath = nullptr;

	SDL_Rect rectMenu;
};

#endif // __MENU_TABS_H__