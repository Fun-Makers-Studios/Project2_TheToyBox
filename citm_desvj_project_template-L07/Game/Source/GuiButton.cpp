#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "TitleScreen.h"
#include "Scene.h"
#include "Fonts.h"
#include "UI.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text, int textSize, ButtonType buttonType_) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
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
		// L15: TODO 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;

			app->fonts->BlitText(bounds.x + (bounds.w / 2) - (9 * (textSize - 1)) / 2, (bounds.y) + (bounds.h / 2), app->ui->font3_id, this->text);

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
			
				app->fonts->BlitText(bounds.x + (bounds.w / 2) - (9 * (textSize - 1)) / 2, (bounds.y) + (bounds.h / 2), app->ui->font3_id, this->text);

			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				NotifyObserver();
			}

		}
		else {
			state = GuiControlState::NORMAL;

			app->fonts->BlitText(bounds.x + (bounds.w / 2) - (9 * (textSize - 1)) / 2, (bounds.y) + (bounds.h / 2) - 4, app->ui->font3_id, this->text);

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
		if(app->render->viewGUIbounds == true)
			render->DrawRectangle(rec, 0, 0, 0, 0);
	} break;

	case GuiControlState::NORMAL:
	{
		if (app->render->viewGUIbounds == true)
			render->DrawRectangle(rec, 255, 255, 0, 255);

		SDL_Rect rect;

		if (buttonType == ButtonType::BIG)
			rect = { 0, 0, 252, 76 };
		else
			rect = { 0, 233, 64, 76 };
		
		render->DrawTexture(buttonTex, rec.x, rec.y, &rect);

	} break;

	//L15: TODO 4: Draw the button according the GuiControl State
	case GuiControlState::FOCUSED:
	{
		if (app->render->viewGUIbounds == true)
			render->DrawRectangle(rec, 255, 255, 255, 160);
		SDL_Rect rect;

		if (buttonType == ButtonType::BIG)
			rect = { 0,75,252,76 };
		else
			rect = { 94, 233, 64, 76 };

		render->DrawTexture(buttonTex, rec.x, rec.y, &rect);

	} break;
	case GuiControlState::PRESSED:
	{
		if (app->render->viewGUIbounds == true)
			render->DrawRectangle(rec, 255, 255, 255, 0);
		SDL_Rect rect;

		if (buttonType == ButtonType::BIG)
			rect = { 0,150,252,76 };
		else
			rect = { 188, 233, 64, 76 };

		render->DrawTexture(buttonTex, rec.x, rec.y, &rect);

	} break;

	case GuiControlState::SELECTED: 
		render->DrawRectangle(rec, 0, 255, 0, 255);
		
		break;

	default:
		break;
	}

	return false;
}