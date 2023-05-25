#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "MenuManager.h"
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

bool GuiButton::Draw(Render* render)
{
	SDL_Rect rec = { bounds.x + app->render->camera.x,  bounds.y + app->render->camera.y, bounds.w, bounds.h };

	// Draw the right button depending on state
	switch (state)
	{

	case GuiControlState::DISABLED: 
	{
		if(app->debug->viewGUIbounds == true)
			render->DrawRectangle(rec, 0, 0, 0, 0);
	} break;

	case GuiControlState::NORMAL:
	{
		if (app->debug->viewGUIbounds == true)
			render->DrawRectangle(rec, 255, 255, 0, 255);

		SDL_Rect rect;

		if (buttonType == ButtonType::BIG)
			rect = { 0, 0, 252, 76 };
		else
			rect = { 0, 233, 64, 76 };
		
		render->DrawTexture(buttonTex, rec.x, rec.y, &rect);
		app->fonts->BlitText(bounds.x + (bounds.w / 2) - (9 * (textSize - 1)) / 2, (bounds.y) + (bounds.h / 2) - 4, app->menuManager->font3_id, this->text);

	} break;

	//L15: TODO 4: Draw the button according the GuiControl State
	case GuiControlState::FOCUSED:
	{
		if (app->debug->viewGUIbounds == true)
			render->DrawRectangle(rec, 255, 255, 255, 160);
		SDL_Rect rect;

		if (buttonType == ButtonType::BIG)
			rect = { 0,75,252,76 };
		else
			rect = { 94, 233, 64, 76 };

		render->DrawTexture(buttonTex, rec.x, rec.y, &rect);
		app->fonts->BlitText(bounds.x + (bounds.w / 2) - (9 * (textSize - 1)) / 2, (bounds.y) + (bounds.h / 2), app->menuManager->font3_id, this->text);

	} break;
	case GuiControlState::PRESSED:
	{
		if (app->debug->viewGUIbounds == true)
			render->DrawRectangle(rec, 255, 255, 255, 0);
		SDL_Rect rect;

		if (buttonType == ButtonType::BIG)
			rect = { 0,150,252,76 };
		else
			rect = { 188, 233, 64, 76 };

		render->DrawTexture(buttonTex, rec.x, rec.y, &rect);
		app->fonts->BlitText(bounds.x + (bounds.w / 2) - (9 * (textSize - 1)) / 2, (bounds.y) + (bounds.h / 2), app->menuManager->font3_id, this->text);

	} break;

	case GuiControlState::SELECTED: 
		render->DrawRectangle(rec, 0, 255, 0, 255);
		
		break;

	default:
		break;
	}

	return false;
}