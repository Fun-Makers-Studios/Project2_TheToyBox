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
	GuiButton* doneQuestsButton = nullptr;
	GuiButton* activeQuestsButton = nullptr;

	enum class ControlID : uint32
	{
		DONE,
		ACTIVE
	};

private:

	SDL_Texture* questMenuImg = nullptr;
	const char* questMenuImgPath = nullptr;
	SDL_Rect rectTexture = { 528, 0, 519, 311 };

	ControlID questListSelected = ControlID::ACTIVE;
	int currentQuestSelectedActive;
	int currentQuestSelectedDone;
};

#endif // __MENU_QUEST_H__