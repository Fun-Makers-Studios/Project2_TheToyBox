#include "GuiInventorySlot.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Textures.h"
//#include "Scene.h"
#include "Fonts.h"

GuiInventorySlot::GuiInventorySlot(uint32 id, SDL_Rect bounds, const char* text, int textSize) : GuiControl(GuiControlType::INVENTORY_SLOT, id)
{
	easing = new Easing(1);
	easing->SetFinished(false);

	this->bounds = bounds;
	this->boundsReset = bounds;
	this->text = text;
	this->textSize = textSize;

	canClick = true;
	drawBasic = false;

	//Load gui button atlas texture
	slotTexPath = app->configNode.child("gui").child("guiSlots").attribute("slotTexPath").as_string();
	slotTex = app->tex->Load(slotTexPath);

}

GuiInventorySlot::~GuiInventorySlot()
{
	delete slotTex;
}

bool GuiInventorySlot::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			if (!selected)
			{
				state = GuiControlState::FOCUSED;

				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT && slotItem != nullptr)
				{
					state = GuiControlState::PRESSED;
				}

				// If mouse button pressed -> Generate event!
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP && slotItem != nullptr)
				{
					selected = !selected;
					state = GuiControlState::SELECTED;
					NotifyObserver();
				}
			}
			else
			{
				state = GuiControlState::SELECTED;
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
				{
					selected = !selected;
					NotifyObserver();
				}
			}
			
		}
		else if(!selected)
		{
			state = GuiControlState::NORMAL;
		}
		else 
		{
			state = GuiControlState::SELECTED;
		}
	}

	return true;
}

bool GuiInventorySlot::Draw(Render* render)
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

	} break;

	//L15: TODO 4: Draw the button according the GuiControl State
	case GuiControlState::FOCUSED:
	{
		if (app->render->viewGUIbounds == true)
			render->DrawRectangle(rec, 255, 255, 255, 160);

	} break;
	case GuiControlState::PRESSED:
	{
		if (app->render->viewGUIbounds == true)
			render->DrawRectangle(rec, 255, 255, 255, 0);

		render->DrawTexture(slotTex, rec.x, rec.y, NULL);
	} break;

	case GuiControlState::SELECTED:
	{
		if (app->render->viewGUIbounds == true)
			render->DrawRectangle(rec, 0, 255, 0, 255);

		render->DrawTexture(slotTex, rec.x, rec.y, NULL);
	} break;

	default:
		break;
	}

	if(slotItem != nullptr)
		render->DrawTexture(slotItem->texture, rec.x, rec.y, NULL);

	return false;
}