#include "ModuleController.h"

#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "Audio.h"
#include "Collisions.h"
#include "Fonts.h"
#include "UI.h"
#include "Player.h"
#include "EntityManager.h"
#include "SceneManager.h"

#include <SDL/include/SDL.h>

#include "Defs.h"
#include "Log.h"

ModuleController::ModuleController():Module()
{
	name.Create("Controller_Module");
}

ModuleController::~ModuleController() 
{}

bool ModuleController::Awake(pugi::xml_node& config)
{
	LOG("Loading UI_Module");
	bool ret = true;



	return ret;
}

bool ModuleController::Start()
{
	LOG("--STARTS CONTROLLER MODULE--");

	SDL_Init(SDL_INIT_GAMECONTROLLER);

	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
		LOG("Error, gamecontroller fail to initilize", SDL_GetError());

	//SDL_GameControllerAddMappingsFromFile(""); -> for adding controll mapping

	return true;
}

bool ModuleController::PreUpdate() 
{
	//Controller inputs

	uint numJoystics = SDL_NumJoysticks();

	for (int i = 0; i < SDL_NumJoysticks(); ++i)
	{
		if (SDL_IsGameController(i))
		{
			if (i == 0 || i == 1 || i == 2 || i == 3)
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
					controller_player1_RightShoulder_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
					controller_player1_Start_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_START);
					controller_player1_B_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_B); ;
					controller_player1_UPArrow_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_DPAD_UP);
					controller_player1_DOWNArrow_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
					controller_player1_LEFTArrow_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
					controller_player1_RIGHTArrow_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

					Controller_player1_Connected = true;
				}
				else
				{
					SDL_GetError();
					SDL_GameControllerClose(Controller_player1);
					Controller_player1 = nullptr;
					Controller_player1_Connected = false;
				}
			}
		}
	}
	//if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_REPEAT) { block.inputX = -1; }

	
	return true;
}

bool ModuleController::Update()
{
	

	return true;
}

bool ModuleController::PostUpdate()
{
	LOG("ModuleController PostUpdate");
	
	//CHECK GAMEPAD INPUT

	//Check player 1 Left Axis X & Y
 	if (Controller_player1_LAxisX > 6400)
	{
		app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT; // HEKATE All of this should probably be '=' not '=='
		
		

	

	}
	else if (Controller_player1_LAxisX < -DEATHZONE)
	{
		app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
	}

	if (Controller_player1_LAxisY < -DEATHZONE)
	{
		app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT;
	}
	else if (Controller_player1_LAxisY > DEATHZONE)
	{
		app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT;
	}



	//Check controller player 1 buttons
	if (controller_player1_A_pressed == true)
	{

		app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;

		app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN;

		


	}    
	

	if (controller_player1_Start_pressed == true && app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN;
	}
	else if (controller_player1_Start_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_REPEAT;
	}

	if (controller_player1_B_pressed == true && app->input->GetKey(SDL_SCANCODE_C) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN;
	}
	else if (controller_player1_B_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT;
	}

	if (controller_player1_UPArrow_pressed == true && app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN;
	}
	else if (controller_player1_UPArrow_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT;
	}

	if (controller_player1_DOWNArrow_pressed == true && app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN;
	}
	else if (controller_player1_DOWNArrow_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT;
	}

	if (controller_player1_LEFTArrow_pressed == true && app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN;
	}
	else if (controller_player1_LEFTArrow_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
	}

	if (controller_player1_RIGHTArrow_pressed == true && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN;
	}
	else if (controller_player1_RIGHTArrow_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;
	}


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