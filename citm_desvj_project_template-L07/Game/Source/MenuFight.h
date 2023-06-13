#ifndef __MENU_FIGHT_H__
#define __MENU_FIGHT_H__

#include "Menu.h"
#include "GuiControl.h"
#include "GuiButton.h"

struct SDL_Texture;

class MenuFight : public Menu
{
public:

	MenuFight();
	virtual ~MenuFight();

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
	GuiButton* attackButton = nullptr;
	GuiButton* specialButton = nullptr;
	GuiButton* bagButton = nullptr;

	enum class ControlID : uint32
	{
		ATTACK,
		SPECIAL,
		BAG
	};

private:

	SDL_Texture* imgPause = nullptr;
	const char* imgPausePath = nullptr;

	SDL_Rect pauseRect;
};

#endif // __MENU_FIGHT_H__