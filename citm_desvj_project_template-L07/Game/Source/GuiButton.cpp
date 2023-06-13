#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "MenuManager.h"
#include "SceneManager.h"
#include "Fonts.h"
#include "Debug.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text, int textSize, ButtonType buttonType_) : GuiControl(GuiControlType::BUTTON, id)
{
	easing = new Easing(1);
	easing->SetFinished(false);

	this->bounds = bounds;
	this->boundsReset = bounds;
	this->text = text;
	this->textSize = textSize;

	canClick = true;
	drawBasic = false;

	buttonType = buttonType_;

	//Load gui button atlas texture
	buttonTexPath = app->configNode.child("gui").child("guiButtons").attribute("buttonsTexPath").as_string();
	buttonTex = app->tex->Load(buttonTexPath);
	
	//buttonTex = app->tex->Load("Assets/Textures/button_texture_atlas.png");

}

GuiButton::~GuiButton()
{
	delete buttonTex;
}

bool GuiButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		// UI Scale for game menus
		if (app->sceneManager->currentScene->id == SceneID::SCENE_GAME || app->sceneManager->currentScene->id == SceneID::SCENE_TITLE)
		{
			mouseX /= 2;
			mouseY /= 2;
		}		

		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;			

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				NotifyObserver();
			}
		}
		else
		{
			state = GuiControlState::NORMAL;
		}
	}

	return true;
}

// Draw the right button depending on state
bool GuiButton::Draw(Render* render)
{
	SDL_Rect rectTexture = GetButtonRect(state, buttonType);
		
	render->DrawTexture(buttonTex, bounds.x, bounds.y, &rectTexture, SDL_FLIP_NONE, ScaleType::UI_200, false);
	app->fonts->BlitText(bounds.x + (bounds.w / 2) - (9 * (textSize - 1)) / 2, (bounds.y) + (bounds.h / 2) - 4, app->menuManager->font3_id, this->text, ScaleType::UI_200);

	return false;
}

SDL_Rect GuiButton::GetButtonRect(GuiControlState guiControlState, ButtonType buttonType)
{
	switch (guiControlState)
	{
		case GuiControlState::NORMAL:
		
			return { 0, 0, 0, 0 };

		case GuiControlState::FOCUSED:

			switch (buttonType)
			{
				case ButtonType::TABS_CLOSED:	return { 0, 0, 25, 26 };
				case ButtonType::TABS_OPEN:		return { 32, 0, 30, 26 };
				case ButtonType::TABS_SELECTED: return { 64, 0, 37, 26 };
				case ButtonType::SQUARE_S:		return { 112, 0, 22, 22 };
				case ButtonType::SQUARE_M:		return { 0, 64, 33, 34 };
				case ButtonType::RECTANGLE_S:	return { 144, 0, 41, 20 };
				case ButtonType::RECTANGLE_M:	return { 192, 0, 60, 20 };
				case ButtonType::UNKNOWN:		return { 0, 0, 0, 0 };
				default: return { 0, 0, 0, 0 };
			}

		case GuiControlState::PRESSED:

			switch (buttonType)
			{
				case ButtonType::TABS_CLOSED:	return { 0, 32, 25, 26 };
				case ButtonType::TABS_OPEN:		return { 32, 32, 30, 26 };
				case ButtonType::TABS_SELECTED: return { 64, 32, 37, 26 };
				case ButtonType::SQUARE_S:		return { 112, 0, 22, 22 };
				case ButtonType::SQUARE_M:		return { 48, 64, 33, 34 };
				case ButtonType::RECTANGLE_S:	return { 144, 32, 41, 20 };
				case ButtonType::RECTANGLE_M:	return { 192, 32, 60, 20 };
				case ButtonType::UNKNOWN:		return { 0, 0, 0, 0 };
				default: return { 0, 0, 0, 0 };
			}

		default: return { 0, 0, 0, 0 };
	}
}