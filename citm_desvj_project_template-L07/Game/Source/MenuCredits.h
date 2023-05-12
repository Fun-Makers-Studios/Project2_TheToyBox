#ifndef __MENU_CREDITS_H__
#define __MENU_CREDITS_H__

#include "Menu.h"
#include "GuiControl.h"
#include "GuiButton.h"

struct SDL_Texture;

class MenuCredits : public Menu
{
public:

	MenuCredits();
	virtual ~MenuCredits();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

private:

	SDL_Texture* popImg_credits = nullptr;
	const char* popImgCreditsPath;

};

#endif // __MENU_CREDITS_H__
