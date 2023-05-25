#include "GuiManager.h"

#include "App.h"
#include "Menu.h"
#include "MenuManager.h"
#include "GuiControl.h"
#include "GuiButton.h"
#include "GuiInventorySlot.h"
#include "GuiSlider.h"

#include "Textures.h"
#include "Audio.h"

GuiManager::GuiManager() : Module()
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	return true;
}


GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, int textSize, SDL_Rect bounds, Menu* observer, ButtonType buttonType, SDL_Rect sliderBounds)
{
	// Create a GUI control and add it to the list of controls
	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
		guiControl = (GuiControl*) new GuiButton(id, bounds, text, textSize, buttonType);
		break;
	case GuiControlType::TOGGLE:
		break;
	case GuiControlType::CHECKBOX:
		break;
	case GuiControlType::SLIDER:
		guiControl = new GuiSlider(id, bounds, text, sliderBounds);
		break;
	case GuiControlType::SLIDERBAR:
		break;
	case GuiControlType::COMBOBOX:
		break;
	case GuiControlType::DROPDOWNBOX:
		break;
	case GuiControlType::INPUTBOX:
		break;
	case GuiControlType::VALUEBOX:
		break;
	case GuiControlType::SPINNER:
		break;
	case GuiControlType::INVENTORY_SLOT:
		guiControl = (GuiControl*) new GuiInventorySlot(id, bounds, text, textSize);
		break;
	default:
		break;
	}

	//Set the observer
	guiControl->SetObserver(observer);

	// Created GuiControls are added to the list of its Menu
	observer->guiControlsList.Add(guiControl);

	return guiControl;
}

bool GuiManager::Update(float dt)
{	
	accumulatedTime += app->GetDT();

	if (accumulatedTime >= updateMsCycle)
		doLogic = true;

	// We control how often the GUI is updated to optimize the performance
	if (doLogic && app->menuManager->currentMenu != nullptr)
	{
		for (size_t i = 0; i < app->menuManager->currentMenu->guiControlsList.Count(); i++)
		{
			ListItem<GuiControl*>* control = app->menuManager->currentMenu->guiControlsList.At(i);
			control->data->Update(dt);
		}

		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}

bool GuiManager::PostUpdate()
{
	

	return true;
}

bool GuiManager::Draw()
{
	if (app->menuManager->currentMenu == nullptr)
		return true;

	ListItem<GuiControl*>* control = app->menuManager->currentMenu->guiControlsList.start;

	while (control != nullptr)
	{
		control->data->Draw(app->render);
		control = control->next;
	}

	return true;

}

void GuiManager::DestroyGuiControl(GuiControl* gui)
{


}

bool GuiManager::CleanUp()
{
	// HEKATE
	/*ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		RELEASE(control);
	}

	return true;

	return false;*/

	return true;

}