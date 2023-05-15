#ifndef __MENU_SETTINGS_H__
#define __MENU_SETTINGS_H__

#include "Menu.h"
#include "GuiControl.h"
#include "GuiButton.h"
#include "GuiSlider.h"


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

	// UI
	GuiButton* decreaseMusicButton = nullptr;
	GuiButton* increaseMusicButton = nullptr;
	GuiButton* decreaseSFXButton = nullptr;
	GuiButton* increaseSFXButton = nullptr;
	GuiButton* fullscreenButton = nullptr;
	GuiButton* vsyncButton = nullptr;

	//Sliders
	GuiSlider* S_music;
	GuiSlider* S_fx;

	enum class ControlID : uint32
	{
		FULL_SCREEN,
		V_SYNC
	};

private:

	SDL_Texture* popImg_settings = nullptr;
	const char* popImgSettingsPath;

	const char* on = "on";
	const char* off = "off";

};

#endif // __MENU_SETTINGS_H__