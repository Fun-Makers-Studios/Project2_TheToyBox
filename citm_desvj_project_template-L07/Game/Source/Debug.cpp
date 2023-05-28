#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Fonts.h"
#include "EntityManager.h"
#include "Map.h"
#include "Pathfinding.h"
#include "Collisions.h"
#include "Debug.h"
#include "SceneManager.h"
#include "MenuManager.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "Defs.h"
#include "Log.h"
#include "Color.h"

#include <string>

Debug::Debug() : Module() 
{
	name.Create("debug");
	debug = false; 
}

Debug::~Debug()
{
	delete nullPlayer;
}

bool Debug::Start()
{
	nullPlayer = new Player();

	debug = false;
	fontID = 3;
	variables = true;
	menus = true;
	camLimits = false;
	godMode = false;
	freeCam = false;
	teleport = false;
	drawColliders = false;
	capFPS = false;

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

		if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
			menus = !menus;

		if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
			teleport = !teleport;

		if (app->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
			fontID++;
	

		// F1: Add functionality
		if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {

		}

		// F2: Add functionality
		if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {

		}

		// F3: Start from the beginning of the current level
		if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
			app->sceneManager->sceneGame->player->ResetPlayerPos();
			app->audio->PlayFx(app->menuManager->selectSFX);
		}

		// F4: Save the current game state
		if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) {
			app->SaveGameRequest();
			app->audio->PlayFx(app->menuManager->selectSFX);
		}

		// F5: Load the previous state (even across levels)
		if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
			app->LoadGameRequest();
			app->audio->PlayFx(app->menuManager->selectSFX);
		}

		// F6: Enables Free Camera Mode
		if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
			freeCam = !freeCam;
		}

		// F7: Draws camera limits center
		if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) {
			camLimits = !camLimits;
		}

		// F8: View GUI bounds rectangles and state in different colors
		if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
			drawGUIbounds = !drawGUIbounds;
			app->audio->PlayFx(app->menuManager->selectSFX);
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
			capFPS = !capFPS;
		}
	}

	return true;
}

bool Debug::PostUpdate()
{
	if (debug)
	{
		if (drawColliders && app->collisions->active)
		{
			DrawColliders();
			DrawEntities();
		}

		if (drawGUIbounds)
		{
			DrawGuiBounds();
		}

		DrawDebug();
	}
		
	return true;
}

void Debug::DrawDebug()
{
	int scale = app->scaleObj->ScaleTypeToInt(app->scaleObj->GetCurrentScale());

	debugX = app->render->camera.w / scale * 0.8;
	debugY = 0;

	lineSpacing = 16;
	lineNum = 0;

	//Debug box black bg
	Color color = Black;
	SDL_Rect rect = { debugX - 16, debugY - 16, 256, 256 };
	app->render->DrawRectangle(rect, color.r, color.g, color.b, 255, false, false);
	app->render->DrawRectangle(rect, color.r, color.g, color.b, 160, true, false);

	debugX *= scale;
	debugY += 32;


	// ==== DEBUG INFO ==== //
	BlitTextDebug("#debug mode (tab) on", 0);
	lineNum++;

	//Free Camera
	if (freeCam)
		BlitTextDebug("#free cam  (f6)   on", 0);
	else
		BlitTextDebug("#free cam  (f6)   off", 0);

	//Cam limits
	if (camLimits)
		BlitTextDebug("#cam lim   (f7)   on", 0);
	else
		BlitTextDebug("#cam lim   (f7)   off", 0);

	//Cam limits
	if (drawGUIbounds)
		BlitTextDebug("#gui debug (f8)   on", 0);
	else
		BlitTextDebug("#gui debug (f8)   off", 0);

	//Draw colliders
	if (drawColliders)
		BlitTextDebug("#colliders (f9)   on", 0);
	else
		BlitTextDebug("#colliders (f9)   off", 0);

	//God Mode
	if (godMode)
		BlitTextDebug("#god mode  (f10)  on", 0);
	else
		BlitTextDebug("#god mode  (f10)  off", 0);

	//FPS Cap
	if (capFPS)
		BlitTextDebug("#cap fps   (f11)  on", 0);
	else
		BlitTextDebug("#cap fps   (f11)  off", 0);


	// VARIABLES
	lineNum++;
	BlitTextDebug("#variables ( v )  on/off", 0);

	if (variables)
	{
		//UI TEST
		/*SDL_Rect rect = { 0, 0, 488, 312 };
		app->render->DrawRectangle(rect, 0, 255, 0, 128, true, false, true);*/

		Player* player;
		if (app->sceneManager->sceneGame->player == nullptr)
			player = nullPlayer;
		else
			player = app->sceneManager->sceneGame->player;

		//Player x, y
		std::string text = std::string("player.x = ") + std::to_string(player->body->pos.x).c_str();
		BlitTextDebug(text, 1);

		text = std::string("player.y = ") + std::to_string(player->body->pos.y).c_str();
		BlitTextDebug(text, 1);

		//Player alive
		if (!player->dead)
			BlitTextDebug("player.alive = true", 1);
		else
			BlitTextDebug("player.alive = false", 1);

		//Camera x, y
		text = std::string("camera.x = ") + std::to_string(app->render->camera.x).c_str();
		BlitTextDebug(text, 1);

		text = std::string("camera.y = ") + std::to_string(app->render->camera.y).c_str();
		BlitTextDebug(text, 1);

		//Scale
		text = app->scaleObj->ScaleTypeToString(app->scaleObj->GetCurrentScale());
		if (!player->dead)
			BlitTextDebug(text, 1);
		else
			BlitTextDebug(text, 1);

		//Font ID
		text = std::string("fontid = ") + std::to_string(fontID).c_str();
		BlitTextDebug(text, 1);
	}


	// MENUS
	lineNum++;
	BlitTextDebug("#menus     ( m )  on/off", 0);

	if (menus)
	{
		ListItem<Menu*>* menuItem = app->menuManager->menus.start;

		while (menuItem != nullptr)
		{
			std::string text = app->menuManager->MenuIDToString(menuItem->data->id) + " - ";

			switch (menuItem->data->menuState)
			{
				case MenuState::ON:
					text += "on";
					break;

				case MenuState::OFF:
					text += "off";
					break;

				case MenuState::SWITCH_ON:
					text += "switch on";
					break;

				case MenuState::SWITCH_OFF:
					text += "switch off";
					break;

				default:
					break;
			}

			BlitTextDebug(text, 1);

			menuItem = menuItem->next;
		}
	}


	// SCENE FIGHT
	if (app->sceneManager->currentScene->id == SceneID::SCENE_FIGHT)
	{		
		lineNum++;
		BlitTextDebug("#fight", 0);

		if (app->sceneManager->sceneFight->turnMember != nullptr)
		{
			std::string text = std::string("turn: ") + app->sceneManager->sceneFight->turnMember->name.GetString();
			BlitTextDebug(text, 1);
		}

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

			std::string text = std::string(member->name.GetString()) + " " + std::to_string(member->currentHp).c_str() + "/" + std::to_string(member->maxHp).c_str() + "hp   " + status;
			BlitTextDebug(text, 1);
		}		
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
	Player* player;
	if (app->sceneManager->sceneGame->player == nullptr)
		player = nullPlayer;
	else
		player = app->sceneManager->sceneGame->player;

	int tileH = app->map->mapData.tileHeight;
	int tileW = app->map->mapData.tileWidth;

	iPoint tilePos = app->map->WorldToMap(player->body->pos.x, player->body->pos.y);

	Color colorCollider;

	// Draw map colliders
	for (int i = 0; i < app->map->mapData.width; i++) //rows
	{
		for (int j = 0; j < app->map->mapData.height; j++) //cols
		{
			if (app->pathfinding->GetTileAt({ i, j }) == 1) //non walkable tiles, 255 invalid walk code
			{
				colorCollider = White;

				// Player proximity 3x3
				if ((tilePos.x - 1 <= i && i <= tilePos.x + 1) &&
					(tilePos.y - 1 <= j && j <= tilePos.y + 1))
				{
					Body tileCollider;
					tileCollider.type = ColliderType::WALL;
					tileCollider.shape = ColliderShape::RECTANGLE;
					tileCollider.mapZone = MapZone::UNKNOWN;
					iPoint iPos = app->map->MapToWorld(i, j);
					tileCollider.pos.x = iPos.x + tileW / 2;
					tileCollider.pos.y = iPos.y + tileH / 2;
					tileCollider.w = tileW;
					tileCollider.h = tileH;
					tileCollider.r = 16;

					if (app->collisions->CheckCollision(*player->body, tileCollider))
						colorCollider = Red;
					else
						colorCollider = Yellow;
				}

				SDL_Rect rect = { i * tileW, j * tileH, tileW, tileH };
				app->render->DrawRectangle(rect, colorCollider.r, colorCollider.g, colorCollider.b, 96);
				app->render->DrawRectangle(rect, colorCollider.r, colorCollider.g, colorCollider.b, 196, false);
			}
		}
	}

	ListItem<Body*>* bodyItem;
	
	for (bodyItem = app->map->mapTeleports.start; bodyItem != NULL; bodyItem = bodyItem->next)
	{
		colorCollider = White;

		// Player proximity 3x3
		if ((player->body->pos.x - bodyItem->data->w <= bodyItem->data->pos.x && bodyItem->data->pos.x <= player->body->pos.x + bodyItem->data->w) &&
			(player->body->pos.y - bodyItem->data->h <= bodyItem->data->pos.y && bodyItem->data->pos.y <= player->body->pos.y + bodyItem->data->h))
		{
			Body tileCollider;
			tileCollider.type = ColliderType::WALL;
			tileCollider.shape = ColliderShape::RECTANGLE;
			tileCollider.mapZone = MapZone::UNKNOWN;
			tileCollider.pos.x = bodyItem->data->pos.x;
			tileCollider.pos.y = bodyItem->data->pos.y;
			tileCollider.w = bodyItem->data->w;
			tileCollider.h = bodyItem->data->h;
			tileCollider.r = 16;

			if (app->collisions->CheckCollision(*player->body, tileCollider))
				colorCollider = Red;
			else
				colorCollider = Yellow;
		}

		SDL_Rect rect = { bodyItem->data->pos.x - bodyItem->data->w/2, bodyItem->data->pos.y - bodyItem->data->h / 2, bodyItem->data->w, bodyItem->data->h };
		app->render->DrawRectangle(rect, colorCollider.r, colorCollider.g, colorCollider.b, 96);
		app->render->DrawRectangle(rect, colorCollider.r, colorCollider.g, colorCollider.b, 196, false);
	}

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
	}
}

void Debug::DrawGuiBounds()
{
	ListItem<Menu*>* menuItem = app->menuManager->menus.start;

	while (menuItem != nullptr)
	{
		if (menuItem->data->menuState == MenuState::ON)
		{
			ListItem<GuiControl*>* control = menuItem->data->guiControlsList.start;

			while (control != nullptr)
			{
				SDL_Rect rec = { control->data->bounds.x,  control->data->bounds.y, control->data->bounds.w, control->data->bounds.h };

				switch (control->data->state)
				{
					case GuiControlState::DISABLED:	app->render->DrawRectangle(rec, 0, 0, 0, 0, true, false, true); break;
					case GuiControlState::NORMAL:	app->render->DrawRectangle(rec, 255, 255, 255, 128, true, false, true); break;
					case GuiControlState::FOCUSED:	app->render->DrawRectangle(rec, 255, 255, 0, 128, true, false, true); break;
					case GuiControlState::PRESSED:	app->render->DrawRectangle(rec, 255, 0, 0, 128, true, false, true); break;
					case GuiControlState::SELECTED: app->render->DrawRectangle(rec, 0, 255, 0, 128, true, false, true); break;
					default: break;
				}
				
				control = control->next;
			}
		}

		menuItem = menuItem->next;
	}

	
}

void Debug::BlitTextDebug(std::string text, uchar tab)
{
	app->fonts->BlitText(debugX + tab * 16, debugY + lineSpacing * lineNum, fontID, text.c_str());
	lineNum++;
}

void Debug::BlitTextDebug(const char* text, uchar tab)
{
	app->fonts->BlitText(debugX + tab * 16, debugY + lineSpacing * lineNum, fontID, text);
	lineNum++;
}