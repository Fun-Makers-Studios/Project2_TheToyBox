#include "GuiSlider.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "Scene.h"
#include "Window.h"
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text, SDL_Rect sliderBounds) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;
	this->sliderbounds = sliderBounds;

	canClick = true;
	drawBasic = false;

	bounds.x = sliderbounds.x + sliderbounds.w;

	focusedFX = app->audio->LoadFx("Assets/Audio/Fx/Menus/open_menu.wav");
	pressedFX = app->audio->LoadFx("Assets/Audio/Fx/Menus/close_menu.wav");
	this->state = GuiControlState::NORMAL;
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(float dt)
{
	//int scale = app->win->GetScale();

	if (canClick == false) {
		state = GuiControlState::NORMAL;
		return false;
	}
	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;

		state = GuiControlState::NORMAL;

		// I'm inside the limits of the button
		if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {

			state = GuiControlState::FOCUSED;
			
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				state = GuiControlState::PRESSED;
				//app->audio->PlayFx(pressed);

			}


			if (previousState == GuiControlState::PRESSED && state == GuiControlState::NORMAL) {
				//in case you want to change the volume in intervals of 25
				/*if (mouseX > sliderbounds.x + 12.5f && mouseX < sliderbounds.x + 37.5f) {
					bounds.x = sliderbounds.x + 25 - bounds.w / 2;
				}else if (mouseX > sliderbounds.x + 37.5f && mouseX < sliderbounds.x + 62.5f) {
					bounds.x = sliderbounds.x + 50 - bounds.w/2;
				}
				else if (mouseX > sliderbounds.x + 62.5f && mouseX < sliderbounds.x + 87.5f) {
					bounds.x = sliderbounds.x + 75 - bounds.w / 2;
				}
				else if(mouseX < sliderbounds.x + 12.5f && mouseX > sliderbounds.x -1){
					bounds.x = sliderbounds.x;
				}
				else if (mouseX > sliderbounds.x + 87.5f && mouseX < sliderbounds.x +sliderbounds.w +1) {
					bounds.x = sliderbounds.x + 100 - bounds.w/2;
				}
				else if (mouseX > sliderbounds.x + 100.0f) {
					bounds.x = sliderbounds.x + sliderbounds.w;
				}*/
			}
			else if (previousState == GuiControlState::NORMAL && state == GuiControlState::FOCUSED) {
				LOG("Change state from %d to %d", previousState, state);
				app->audio->PlayFx(focusedFX);
			}

			if (state == GuiControlState::FOCUSED && state == GuiControlState::PRESSED)
				app->audio->PlayFx(pressedFX);

			if (state == GuiControlState::PRESSED) {
				bounds.x = (mouseX - (bounds.w / 4));
				if (bounds.x > sliderbounds.x + sliderbounds.w) {
					bounds.x = sliderbounds.x + sliderbounds.w;
				}
				else if (bounds.x < sliderbounds.x) {
					bounds.x = sliderbounds.x;
				}
			}

			if (slider == GuiSliderType::MUSIC) {
				Mix_VolumeMusic(bounds.x - sliderbounds.x);
				app->musicValue = bounds.x - sliderbounds.x;
			}
			if (slider == GuiSliderType::FX) {
				Mix_Volume(-1, bounds.x - sliderbounds.x);
				app->musicValue = bounds.x - sliderbounds.x;
			}
		}
	}
}

bool GuiSlider::Draw(Render* render)
{
	//L15: DONE 4: Draw the button according the GuiControl State
	app->input->GetMousePosition(mouseX, mouseY);

			switch (state)
			{
			case GuiControlState::DISABLED:

				app->render->DrawRectangle(sliderbounds, 255, 255, 255, 255, true, false, true);
				app->render->DrawRectangle({ bounds.x,bounds.y,bounds.w,bounds.h }, 0, 0, 0, 255, true, false, true);

				break;
			case GuiControlState::NORMAL:
				app->render->DrawRectangle(sliderbounds, 135, 106, 89, 255, true, false, true);
				app->render->DrawRectangle({ bounds.x,bounds.y,bounds.w,bounds.h }, 107, 80, 82, 255, true, false, true);

				break;
			case GuiControlState::FOCUSED:
				//section of the rectangle
				app->render->DrawRectangle(sliderbounds, 135, 106, 89, 255, true, false, true);
				app->render->DrawRectangle({ bounds.x,bounds.y,bounds.w,bounds.h }, 169, 96, 70, 255, true, false, true);

				break;
			case GuiControlState::PRESSED:
				app->render->DrawRectangle(sliderbounds, 177, 138, 116, 255, true, false, true);
				app->render->DrawRectangle({ bounds.x,bounds.y,bounds.w,bounds.h }, 199, 140, 99, 255, true, false, true);

				break;
			}

	return false;
}