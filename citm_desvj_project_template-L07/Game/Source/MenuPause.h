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
	GuiButton* decreaseMusicButton = nullptr;
	GuiButton* increaseMusicButton = nullptr;
	GuiButton* decreaseSFXButton = nullptr;
	GuiButton* increaseSFXButton = nullptr;
	GuiButton* fullscreenButton = nullptr;
	GuiButton* vsyncButton = nullptr;

	enum class ControlID : uint32
	{
		FULL_SCREEN,
		V_SYNC
	};

private:

	SDL_Texture* popImg_credits = nullptr;
	const char* popImgCreditsPath;

};

#endif // __MENU_PAUSE_H__