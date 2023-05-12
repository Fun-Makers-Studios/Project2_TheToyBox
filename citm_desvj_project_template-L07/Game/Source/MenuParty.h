#ifndef __MENU_PARTY_H__
#define __MENU_PARTY_H__

#include "Menu.h"
#include "GuiControl.h"
#include "GuiButton.h"

struct SDL_Texture;

class MenuParty : public Menu
{
public:

	MenuParty();
	virtual ~MenuParty();

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
	GuiButton* partyMember1Button = nullptr;
	GuiButton* partyMember2Button = nullptr;

	enum class ControlID : uint32
	{
		PARTY_1,
		PARTY_2
	};

private:

	ControlID partyMemberSelected = ControlID::PARTY_1;

	//Textures
	SDL_Texture* partyMenuImg = nullptr;
	const char* partyMenuImgPath = nullptr;

	const char* zeroImgPath = nullptr;
	SDL_Texture* zeroImg = nullptr;

	const char* sophieImgPath = nullptr;
	SDL_Texture* sophieImg = nullptr;

};

#endif // __MENU_PARTY_H__