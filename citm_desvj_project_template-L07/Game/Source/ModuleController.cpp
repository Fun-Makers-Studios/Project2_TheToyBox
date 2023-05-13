#include "ModuleController.h"

#include "app.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "Audio.h"
#include "Collisions.h"
#include "Fonts.h"
#include "UI.h"

#include <SDL/include/SDL.h>

#include "Defs.h"
#include "Log.h"


ModuleController::ModuleController() : Module()
{
	name.Create("controller");
}

ModuleController::~ModuleController() {}

bool ModuleController::Awake(pugi::xml_node& config)
{
	LOG("AWAKE gamepads");

	return true;
}

bool ModuleController::Start()
{
	LOG("START gamepads");

	SDL_Init(SDL_INIT_GAMECONTROLLER);

	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
		LOG("Error, gamecontroller fail to initilize", SDL_GetError());

	//SDL_GameControllerAddMappingsFromFile(""); -> for adding controll mapping

	return true;
}


bool ModuleController::PreUpdate()
{
	//Controller inputs

	for (int i = 0; i < SDL_NumJoysticks(); ++i)
	{
		if (SDL_IsGameController(i))
		{
			if (i == 0)
			{
				Controller_player1 = SDL_GameControllerOpen(i);
				//Checking if the controller is attached

				if (SDL_GameControllerGetAttached(Controller_player1))
				{
					//Assign values of the aixis to Controller_Player1_LAxis...
					Controller_player1_LAxisX = SDL_GameControllerGetAxis(Controller_player1, SDL_CONTROLLER_AXIS_LEFTX);
					Controller_player1_LAxisY = SDL_GameControllerGetAxis(Controller_player1, SDL_CONTROLLER_AXIS_LEFTY);

					//Assign values of the aixis to Controller_Player1_RAxis...
					Controller_player1_RAxisX = SDL_GameControllerGetAxis(Controller_player1, SDL_CONTROLLER_AXIS_RIGHTX);
					Controller_player1_RAxisY = SDL_GameControllerGetAxis(Controller_player1, SDL_CONTROLLER_AXIS_RIGHTY);


					//Assign the boolean value to the Button bools defined
					controller_player1_A_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_A);
					controller_player1_B_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_B);
					controller_player1_X_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_X);
					controller_player1_Y_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_Y);

					controller_player1_Start_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_START);
					controller_player1_Back_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_BACK);
					controller_player1_Home_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_GUIDE);

					controller_player1_R1_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
					controller_player1_L1_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
					
					controller_player1_L3_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_LEFTSTICK);
					controller_player1_R3_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_RIGHTSTICK);
					
					controller_player1_UPArrow_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_DPAD_UP);
					controller_player1_DOWNArrow_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
					controller_player1_LEFTArrow_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
					controller_player1_RIGHTArrow_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

					Controller_player1_Connected = true;
				}
				else
				{
					SDL_GameControllerClose(Controller_player1);
					Controller_player1 = nullptr;
					Controller_player1_Connected = false;
				}
			}
		}
	}
	//if (app->input->keyboard[SDL_SCANCODE_LEFT] == KeyState::KEY_REPEAT) { block.inputX = -1; }

	//CHECK LEFT JOYSTICK
	if (Controller_player1_LAxisX > 6400)
	{
		//Move Right
		app->input->keyboard[SDL_SCANCODE_D] = KeyState::KEY_REPEAT;
	}
	else if (Controller_player1_LAxisX < -DEATHZONE)
	{
		//Move Left
		app->input->keyboard[SDL_SCANCODE_A] = KeyState::KEY_REPEAT;
	}
	if (Controller_player1_LAxisY < -DEATHZONE)
	{
		//Move Up
		app->input->keyboard[SDL_SCANCODE_W] = KeyState::KEY_REPEAT;
	}
	else if (Controller_player1_LAxisY > DEATHZONE)
	{
		//Move Down
		app->input->keyboard[SDL_SCANCODE_S] = KeyState::KEY_REPEAT;
	}

	//CHECK RIGHT JOYSTICK
	/*if (Controller_player1_RAxisX > 6400)
	{
		app->input->keyboard[SDL_SCANCODE_] = KeyState::KEY_REPEAT;
	}
	else if (Controller_player1_RAxisX < -DEATHZONE)
	{
		app->input->keyboard[SDL_SCANCODE_] = KeyState::KEY_REPEAT;
	}

	if (Controller_player1_RAxisY < -DEATHZONE)
	{
		app->input->keyboard[SDL_SCANCODE_] = KeyState::KEY_REPEAT;
	}
	else if (Controller_player1_RAxisY > DEATHZONE)
	{
		app->input->keyboard[SDL_SCANCODE_] = KeyState::KEY_REPEAT;
	}*/


	//CHECK BUTTONS
	// A Button
	if (controller_player1_A_pressed == true && app->input->keyboard[SDL_SCANCODE_RETURN] == KeyState::KEY_IDLE)
	{
		app->input->keyboard[SDL_SCANCODE_RETURN] = KeyState::KEY_DOWN;
	}
	else if (controller_player1_A_pressed == true)
	{
		app->input->keyboard[SDL_SCANCODE_RETURN] = KeyState::KEY_REPEAT;
	}

	// B Button 
	if (controller_player1_B_pressed == true && app->input->keyboard[SDL_SCANCODE_ESCAPE] == KeyState::KEY_IDLE)
	{
		// Open Pause Menu/Close Menu(s)
		app->input->keyboard[SDL_SCANCODE_ESCAPE] = KeyState::KEY_DOWN;
	}
	else if (controller_player1_B_pressed == true)
	{
		app->input->keyboard[SDL_SCANCODE_ESCAPE] = KeyState::KEY_REPEAT;
	}

	// X Button
	if (controller_player1_X_pressed == true && app->input->keyboard[SDL_SCANCODE_G] == KeyState::KEY_IDLE)
	{
		// Talk with NPCs
		app->input->keyboard[SDL_SCANCODE_G] = KeyState::KEY_DOWN;
	}
	else if (controller_player1_X_pressed == true)
	{
		app->input->keyboard[SDL_SCANCODE_G] = KeyState::KEY_REPEAT;
	}

	// Y Button
	/*if (controller_player1_Y_pressed == true && app->input->keyboard[SDL_SCANCODE_] == KeyState::KEY_IDLE)
	{
		app->input->keyboard[SDL_SCANCODE_] = KeyState::KEY_DOWN;
	}
	else if (controller_player1_Y_pressed == true)
	{
		app->input->keyboard[SDL_SCANCODE_] = KeyState::KEY_REPEAT;
	}*/


	// START Button
	if (controller_player1_Start_pressed == true && app->input->keyboard[SDL_SCANCODE_T] == KeyState::KEY_IDLE)
	{
		//Open TeamParty/Inventory Menu
		app->input->keyboard[SDL_SCANCODE_T] = KeyState::KEY_DOWN;
	}
	else if (controller_player1_Start_pressed == true)
	{
		app->input->keyboard[SDL_SCANCODE_T] = KeyState::KEY_REPEAT;
	}

	// BACK Button
	if (controller_player1_Back_pressed == true && app->input->keyboard[SDL_SCANCODE_J] == KeyState::KEY_IDLE)
	{
		//Open Quests Menu
		app->input->keyboard[SDL_SCANCODE_J] = KeyState::KEY_DOWN;
	}
	else if (controller_player1_Back_pressed == true)
	{
		app->input->keyboard[SDL_SCANCODE_J] = KeyState::KEY_REPEAT;
	}

	// HOME Button
	/*if (controller_player1_Home_pressed == true && app->input->keyboard[SDL_SCANCODE_] == KeyState::KEY_IDLE)
	{
		app->input->keyboard[SDL_SCANCODE_] = KeyState::KEY_DOWN;
	}
	else if (controller_player1_Home_pressed == true)
	{
		app->input->keyboard[SDL_SCANCODE_] = KeyState::KEY_REPEAT;
	}*/

	// L1 Button
	/*if (controller_player1_L1_pressed == true && app->input->keyboard[SDL_SCANCODE_] == KeyState::KEY_IDLE)
	{
		app->input->keyboard[SDL_SCANCODE_] = KeyState::KEY_DOWN;
	}
	else if (controller_player1_L1_pressed == true)
	{
		app->input->keyboard[SDL_SCANCODE_] = KeyState::KEY_REPEAT;
	}*/

	// R1 Button
	/*if (controller_player1_R1_pressed == true && app->input->keyboard[SDL_SCANCODE_] == KeyState::KEY_IDLE)
	{
		app->input->keyboard[SDL_SCANCODE_] = KeyState::KEY_DOWN;
	}
	else if (controller_player1_R1_pressed == true)
	{
		app->input->keyboard[SDL_SCANCODE_] = KeyState::KEY_REPEAT;
	}*/

	// L3 Button
	/*if (controller_player1_L3_pressed == true && app->input->keyboard[SDL_SCANCODE_] == KeyState::KEY_IDLE)
	{
		app->input->keyboard[SDL_SCANCODE_] = KeyState::KEY_DOWN;
	}
	else if (controller_player1_L3_pressed == true)
	{
		app->input->keyboard[SDL_SCANCODE_] = KeyState::KEY_REPEAT;
	}*/

	// R3 Button
	/*if (controller_player1_R3_pressed == true && app->input->keyboard[SDL_SCANCODE_] == KeyState::KEY_IDLE)
	{
		app->input->keyboard[SDL_SCANCODE_] = KeyState::KEY_DOWN;
	}
	else if (controller_player1_R3_pressed == true)
	{
		app->input->keyboard[SDL_SCANCODE_] = KeyState::KEY_REPEAT;
	}*/


	// UP ARROW Button
	if (controller_player1_UPArrow_pressed == true && app->input->keyboard[SDL_SCANCODE_UP] == KeyState::KEY_IDLE)
	{
		app->input->keyboard[SDL_SCANCODE_UP] = KeyState::KEY_DOWN;
	}
	else if (controller_player1_UPArrow_pressed == true)
	{
		app->input->keyboard[SDL_SCANCODE_UP] = KeyState::KEY_REPEAT;
	}

	// DOWN ARROW Button
	if (controller_player1_DOWNArrow_pressed == true && app->input->keyboard[SDL_SCANCODE_DOWN] == KeyState::KEY_IDLE)
	{
		app->input->keyboard[SDL_SCANCODE_DOWN] = KeyState::KEY_DOWN;
	}
	else if (controller_player1_DOWNArrow_pressed == true)
	{
		app->input->keyboard[SDL_SCANCODE_DOWN] = KeyState::KEY_REPEAT;
	}

	// LEFT ARROW Button
	if (controller_player1_LEFTArrow_pressed == true && app->input->keyboard[SDL_SCANCODE_LEFT] == KeyState::KEY_IDLE)
	{
		app->input->keyboard[SDL_SCANCODE_LEFT] = KeyState::KEY_DOWN;
	}
	else if (controller_player1_LEFTArrow_pressed == true)
	{
		app->input->keyboard[SDL_SCANCODE_LEFT] = KeyState::KEY_REPEAT;
	}

	// RIGHT ARROW Button
	if (controller_player1_RIGHTArrow_pressed == true && app->input->keyboard[SDL_SCANCODE_RIGHT] == KeyState::KEY_IDLE)
	{
		app->input->keyboard[SDL_SCANCODE_RIGHT] = KeyState::KEY_DOWN;
	}
	else if (controller_player1_RIGHTArrow_pressed == true)
	{
		app->input->keyboard[SDL_SCANCODE_RIGHT] = KeyState::KEY_REPEAT;
	}

	return true;
}

bool ModuleController::Update()
{
	return true;
}

bool ModuleController::PostUpdate()
{
	return true;
}


// Called before quitting
bool  ModuleController::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	SDL_GameControllerClose(Controller_player1);

	Controller_player1 = nullptr;

	return true;
}