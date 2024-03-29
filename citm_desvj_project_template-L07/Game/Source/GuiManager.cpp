#include "GuiManager.h"
#include "App.h"
#include "Textures.h"

#include "GuiButton.h"
#include "Audio.h"

GuiManager::GuiManager() :Module()
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, int textSize, SDL_Rect bounds, Module* observer, ButtonType buttonType, SDL_Rect sliderBounds)
{
	// L15: TODO1: Create a GUI control and add it to the list of controls

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
	default:
		break;
	}

	//Set the observer
	guiControl->SetObserver(observer);

	// Created GuiControls are add it to the list of controls
	guiControlsList.Add(guiControl);

	return guiControl;
}

bool GuiManager::Update(float dt)
{	
	
	
	return true;
}

bool GuiManager::PostUpdate()
{
	accumulatedTime += app->GetDT();
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	// We control how often the GUI is updated to optimize the performance
	if (doLogic == true)
	{
		ListItem<GuiControl*>* control = guiControlsList.start;

		while (control != nullptr)
		{
			control->data->Update(app->GetDT());
			control = control->next;
		}

		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}

bool GuiManager::Draw() {

	ListItem<GuiControl*>* control = guiControlsList.start;

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
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		RELEASE(control);
	}

	return true;

	return false;
}



