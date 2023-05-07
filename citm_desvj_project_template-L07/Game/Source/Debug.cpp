#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Fonts.h"
#include "EntityManager.h"
#include "Map.h"
#include "Collisions.h"
#include "Debug.h"
#include "SceneManager.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Defs.h"
#include "Log.h"
#include "Color.h"

#include <string>

Debug::Debug() : Module() { debug = false; }
Debug::~Debug()
{
	delete nullPlayer;
}

bool Debug::Start()
{
	nullPlayer = new Player();
	debug = false;
	variables = true;
	desiredFPS = 60;
	return true;
}

bool Debug::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
		debug = !debug;

	if (debug)
	{
		if (app->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
			variables = !variables;

		if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
			camLimits = !camLimits;

		if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
			teleport = !teleport;

		if (app->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
			freeCam = !freeCam;

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			drawEntities = !drawEntities;

	}
	// F1: Add functionality
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {

	}
	// F2: Add functionality
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {

	}

	// F3: Start from the beginning of the current level
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		app->sceneManager->sceneGame->player->ResetPlayerPos();
		app->audio->PlayFx(app->sceneManager->sceneGame->player->selectSFX);
	}

	// F5: Save the current game state
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		app->SaveGameRequest();
		app->audio->PlayFx(app->sceneManager->sceneGame->player->selectSFX);
	}

	// F6: Load the previous state (even across levels)
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		app->LoadGameRequest();
		app->audio->PlayFx(app->sceneManager->sceneGame->player->selectSFX);
	}

	// F8: View GUI bounds rectangles and state in different colors
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		app->render->viewGUIbounds = !app->render->viewGUIbounds;
		app->audio->PlayFx(app->sceneManager->sceneGame->player->selectSFX);
	}

	// F9: View colliders / logic / paths
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		drawColliders = !drawColliders;
	}

	// F10: God Mode (fly around, cannot be killed, etc)
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		godMode = !godMode;
	}

	// F11: Enable/Disable FPS cap to xml value
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		controlFPS = !controlFPS;
	}

	return true;
}

bool Debug::PostUpdate()
{
	if (debug)
	{
		if (drawColliders && app->collisions->active)
			DrawColliders();

		if (drawEntities)
			DrawEntities();

		DrawDebug();
	}
		
	return true;
}

void Debug::DrawDebug()
{
	int scale = app->win->GetScale();

	int debugX = app->render->camera.w / scale * 0.7;
	int debugY = app->render->camera.h / scale * 0.1;

	//Debug box black bg
	Color color = Black;
	SDL_Rect rect = { debugX - 16, debugY - 16, 256, 256 };
	app->render->DrawRectangle(rect, color.r, color.g, color.b, 255, false, false);
	app->render->DrawRectangle(rect, color.r, color.g, color.b, 160, true, false);


	app->fonts->BlitText(debugX, debugY, 0, "#debug mode (tab)  on/off");

	//Free Camera
	if (drawColliders)
		app->fonts->BlitText(debugX, debugY + 15, 0, "#colliders (f9)  on");
	else
		app->fonts->BlitText(debugX, debugY + 15, 0, "#colliders (f9)  off");
	
	//God Mode
	if (godMode)
		app->fonts->BlitText(debugX, debugY + 30, 0, "#god mode  (f10)  on");
	else
		app->fonts->BlitText(debugX, debugY + 30, 0, "#god mode  (f10)  off");


	//Variables
	app->fonts->BlitText(debugX, debugY, 45, "#variables ( V )  on/off");

	if (variables)
	{
		Player* player;
		if (app->sceneManager->sceneGame->player == nullptr)
			player = nullPlayer;
		else
			player = app->sceneManager->sceneGame->player;

		//Player x, y
		app->fonts->BlitText(debugX, debugY + 55, 0, "player.x =");
			app->fonts->BlitText(debugX + 88, debugY + 55, 0, std::to_string(player->body->pos.x).c_str());

		app->fonts->BlitText(debugX, debugY + 65, 0, "player.y =");
		app->fonts->BlitText(debugX + 88, debugY + 65, 0, std::to_string(player->body->pos.y).c_str());

		//Camera x, y
		app->fonts->BlitText(debugX, debugY + 80, 0, "camera.x =");
		app->fonts->BlitText(debugX + 88, debugY + 80, 0, std::to_string(app->render->camera.x).c_str());

		app->fonts->BlitText(debugX, debugY + 90, 0, "camera.y =");
		app->fonts->BlitText(debugX + 88, debugY + 90, 0, std::to_string(app->render->camera.y).c_str());

		//Player alive
		if (!player->dead)
			app->fonts->BlitText(debugX, debugY + 105, 0, "player.alive = true");
		else
			app->fonts->BlitText(debugX, debugY + 105, 0, "player.alive = false");

		//Scene Fight
		for (size_t i = 0; i < app->sceneManager->sceneFight->turnList.Count(); i++)
		{
			PartyMember* member = app->sceneManager->sceneFight->turnList.At(i)->data;

			std::string status;
			switch (member->status)
			{
			case MemberStatus::NORMAL: status = "normal"; break;
			case MemberStatus::POISONED: status = "poisoned"; break;
			case MemberStatus::STUNED: status = "stuned"; break;
			case MemberStatus::DEAD: status = "dead"; break;
			default: break;
			}

			std::string strName = std::string(member->name.GetString()); 
			std::string strInfo = std::string(std::to_string(member->currentHp).c_str()) + "/" + std::to_string(member->maxHp).c_str() + "hp   " + status;

			app->fonts->BlitText(debugX, debugY + 125 + i*10, 0, strName.c_str());
			app->fonts->BlitText(debugX+88, debugY + 125 + i*10, 0, strInfo.c_str());
		}

		if (app->sceneManager->sceneFight->turnMember != nullptr)
		{
			app->fonts->BlitText(debugX - 250, debugY, 0, app->sceneManager->sceneFight->turnMember->name.GetString());
		}


		//Scale
		std::string currentScale = app->scaleObj->ScaleTypeToString(app->scaleObj->GetCurrentScale());
		if (!player->dead)
			app->fonts->BlitText(debugX, debugY + 200, 0, currentScale.c_str());
		else
			app->fonts->BlitText(debugX, debugY + 200, 0, currentScale.c_str());
		
	}

	//Camera limits
	if (camLimits)
	{
		app->render->DrawLine(app->render->camera.w / 2, 0, app->render->camera.w / 2, app->render->camera.h, 0, 0, 255, 255, false);
		app->render->DrawLine(0, app->render->camera.h / 2, app->render->camera.w, app->render->camera.h / 2, 0, 0, 255, 255, false);
	}

	//Teleport
	if (teleport)
	{
		/*if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			app->scene->player->position.x = 2120;
			app->scene->player->position.y = 385;
		}*/
	}
}

void Debug::DrawColliders()
{
	// HEKATE MUST DRAW COLLIDERS
	// Currently at Collisions Update()
}

void Debug::DrawEntities()
{
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = app->entityManager->entities.start; item != NULL; item = item->next)
	{
		Color color;

		pEntity = item->data;

		switch (pEntity->type)
		{
		case EntityType::PLAYER:		color = Magenta;	break;
		case EntityType::NPC:			color = Blue;		break;
		case EntityType::ENEMY_KID:		color = Red;		break;
		case EntityType::ITEM:			color = Green;		break;
		default:						color = White;		break;
		}

		if (pEntity->body != nullptr)
		{
			app->render->DrawCircle(
				pEntity->body->pos.x,
				pEntity->body->pos.y,
				pEntity->body->r,
				color.r, color.g, color.b, 255);
		}
		


		/*SDL_Rect rect = { pEntity->body->pos.x, pEntity->body->pos.y, 32, 32};
		app->render->DrawRectangle(rect, color.r, color.g, color.b, 255, false);
		app->render->DrawRectangle(rect, color.r, color.g, color.b, 64, true);*/
	}
}