#ifndef __MENU_QUEST_H__
#define __MENU_QUEST_H__

#include "Menu.h"
#include "GuiControl.h"
#include "GuiButton.h"

struct SDL_Texture;

class MenuQuest : public Menu
{
public:

	MenuQuest();
	virtual ~MenuQuest();

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
	GuiButton* quest1 = nullptr;
	GuiButton* quest2 = nullptr;
	GuiButton* quest3 = nullptr;
	GuiButton* quest4 = nullptr;
	GuiButton* quest5 = nullptr;
	GuiButton* quest6 = nullptr;
	GuiButton* quest7 = nullptr;

	enum class ControlID : uint32
	{
		QUEST_1,
		QUEST_2,
		QUEST_3,
		QUEST_4,
		QUEST_5,
		QUEST_6,
		QUEST_7
	};

private:

	SDL_Texture* questMenuImg = nullptr;
	const char* questMenuImgPath = nullptr;
	SDL_Rect rectTexture = { 528, 0, 519, 311 };

	int currentQuestSelectedID;
	uint fontID;
};

#endif // __MENU_QUEST_H__