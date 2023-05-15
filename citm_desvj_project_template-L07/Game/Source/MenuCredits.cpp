#include "MenuCredits.h"

#include "App.h"
#include "Menu.h"
#include "SceneManager.h"
#include "MenuManager.h"
#include "GuiManager.h"
#include "UI.h"

#include "Window.h"
#include "Audio.h"
#include "Fonts.h"
#include "Log.h"


MenuCredits::MenuCredits() : Menu()
{
	id = MenuID::MENU_CREDITS;

	popImgCreditsPath = app->configNode.child("menuManager").child("popImage").attribute("creditstexturepath").as_string();
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
	popImg_credits = app->tex->Load(popImgCreditsPath);

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
	app->render->DrawRectangle({ 0, 0, app->render->camera.w, app->render->camera.w }, 0, 0, 0, 128, true, false, true);
	
	app->render->DrawTexture(popImg_credits, 0, 0, NULL);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->audio->PlayFx(app->menuManager->closeMenuSFX);
		app->menuManager->SetDefaultMenu();
	}

	return true;
}


bool MenuCredits::PostUpdate()
{
	bool ret = true;

	return ret;
}


bool MenuCredits::CleanUp()
{
	LOG("Freeing MenuCredits");

	app->tex->UnLoad(popImg_credits);

	//STORE IN A LIST THIS BUTTONS AND THEN CHECK HERE IF NULLPTR TO CLEAN THEM UP
	//guiControlsList.Clear();

	return true;
}

bool MenuCredits::OnGuiMouseClickEvent(GuiControl* control)
{
	return true;
}