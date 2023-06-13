#include "MenuCredits.h"

#include "App.h"
#include "Menu.h"
#include "SceneManager.h"
#include "MenuManager.h"
#include "GuiManager.h"

#include "Window.h"
#include "Audio.h"
#include "Fonts.h"
#include "Log.h"


MenuCredits::MenuCredits() : Menu()
{
	id = MenuID::MENU_CREDITS;

	creditsMenuImgPath = app->configNode.child("menuManager").child("menuCredits").attribute("texturepath").as_string();
}


MenuCredits::~MenuCredits()
{}


bool MenuCredits::Awake(pugi::xml_node& config)
{
	LOG("Loading MenuPause");
	bool ret = true;

	return ret;
}


bool MenuCredits::Start()
{
	LOG("--STARTS PAUSE MENU--");

	// Load
	creditsMenuImg = app->tex->Load(creditsMenuImgPath);

	// Set easing finished on title buttons
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		if (control->data->id < 5)
		{
			control->data->easing->SetFinished(false);
			control->data->easing->SetTotalTime(1 + 0.2 * control->data->id);
		}

		control = control->next;
	}

	return true;
}


bool MenuCredits::PreUpdate()
{
	return true;
}


bool MenuCredits::Update(float dt)
{

	return true;
}


bool MenuCredits::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(creditsMenuImg, app->menuManager->openBookPos.x, app->menuManager->openBookPos.y, &rectMenu, SDL_FLIP_NONE, ScaleType::UI_200, false);


	return ret;
}


bool MenuCredits::CleanUp()
{
	LOG("Freeing MenuCredits");

	app->tex->UnLoad(creditsMenuImg);

	//STORE IN A LIST THIS BUTTONS AND THEN CHECK HERE IF NULLPTR TO CLEAN THEM UP
	//guiControlsList.Clear();

	return true;
}

bool MenuCredits::OnGuiMouseClickEvent(GuiControl* control)
{
	return true;
}