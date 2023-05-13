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
					//Assign values of the aixis to Controller_Player1_LAxisX...
					Controller_player1_LAxisX = SDL_GameControllerGetAxis(Controller_player1, SDL_CONTROLLER_AXIS_LEFTX);
					Controller_player1_LAxisY = SDL_GameControllerGetAxis(Controller_player1, SDL_CONTROLLER_AXIS_LEFTY);

					//Assign the boolean value to the  bools defined  
					controller_player1_A_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_A);
					//controller_player1_RightShoulder_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
					controller_player1_Start_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_START);
					controller_player1_B_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_B); ;

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

	//Check player 1 Left Axis X & Y
	if (Controller_player1_LAxisX > 6400)
	{
		app->input->keyboard[SDL_SCANCODE_D] = KeyState::KEY_REPEAT;
	}
	else if (Controller_player1_LAxisX < -DEATHZONE)
	{
		app->input->keyboard[SDL_SCANCODE_A] = KeyState::KEY_REPEAT;
	}

	if (Controller_player1_LAxisY < -DEATHZONE)
	{
		app->input->keyboard[SDL_SCANCODE_W] = KeyState::KEY_REPEAT;
	}
	else if (Controller_player1_LAxisY > DEATHZONE)
	{
		app->input->keyboard[SDL_SCANCODE_S] = KeyState::KEY_REPEAT;
	}

	//Check controller player 1 buttons
	if (controller_player1_A_pressed == true && app->input->keyboard[SDL_SCANCODE_T] == KeyState::KEY_IDLE)
	{
		app->input->keyboard[SDL_SCANCODE_T] = KeyState::KEY_DOWN;
	}
	else if (controller_player1_A_pressed == true)
	{
		app->input->keyboard[SDL_SCANCODE_T] = KeyState::KEY_REPEAT;
	}

	if (controller_player1_Start_pressed == true && app->input->keyboard[SDL_SCANCODE_C] == KeyState::KEY_IDLE)
	{
		app->input->keyboard[SDL_SCANCODE_C] = KeyState::KEY_DOWN;
	}
	else if (controller_player1_Start_pressed == true)
	{
		app->input->keyboard[SDL_SCANCODE_C] = KeyState::KEY_REPEAT;
	}

	if (controller_player1_B_pressed == true && app->input->keyboard[SDL_SCANCODE_ESCAPE] == KeyState::KEY_IDLE)
	{
		app->input->keyboard[SDL_SCANCODE_ESCAPE] = KeyState::KEY_DOWN;
	}
	else if (controller_player1_B_pressed == true)
	{
		app->input->keyboard[SDL_SCANCODE_ESCAPE] = KeyState::KEY_REPEAT;
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