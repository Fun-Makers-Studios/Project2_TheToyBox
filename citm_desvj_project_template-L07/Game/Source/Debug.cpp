#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Fonts.h"
#include "EntityManager.h"
#include "Map.h"
#include "Physics.h"
#include "ModuleFadeToBlack.h"
#include "Debug.h"
#include "LogoScreen.h"
#include "TitleScreen.h"
#include "Scene.h"
#include "SceneFight.h"
#include "EndingScreen.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Defs.h"
#include "Log.h"
#include "Color.h"

#include <string>

Debug::Debug() : Module() { debug = false; }
Debug::~Debug() {}

bool Debug::Start()
{
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

		if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
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
		app->scene->player->ResetPlayerPos();
		app->audio->PlayFx(app->scene->player->selectSFX);
	}

	// F5: Save the current game state
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		app->SaveGameRequest();
		app->audio->PlayFx(app->scene->player->selectSFX);
	}

	// F6: Load the previous state (even across levels)
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		app->LoadGameRequest();
		app->audio->PlayFx(app->scene->player->selectSFX);
	}

	// F8: View GUI bounds rectangles and state in different colors
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		app->render->viewGUIbounds = !app->render->viewGUIbounds;
		app->audio->PlayFx(app->scene->player->selectSFX);
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
		if (drawColliders && app->physics->active)
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

		//Player x, y
		app->fonts->BlitText(debugX, debugY + 55, 0, "player.x =");
		app->fonts->BlitText(debugX + 88, debugY + 55, 0, std::to_string(app->scene->player->position.x).c_str());

		app->fonts->BlitText(debugX, debugY + 65, 0, "player.y =");
		app->fonts->BlitText(debugX + 88, debugY + 65, 0, std::to_string(app->scene->player->position.y).c_str());

		//Camera x, y
		app->fonts->BlitText(debugX, debugY + 80, 0, "camera.x =");
		app->fonts->BlitText(debugX + 88, debugY + 80, 0, std::to_string(app->render->camera.x).c_str());

		app->fonts->BlitText(debugX, debugY + 90, 0, "camera.y =");
		app->fonts->BlitText(debugX + 88, debugY + 90, 0, std::to_string(app->render->camera.y).c_str());

		//Player alive
		if (!app->scene->player->dead)
			app->fonts->BlitText(debugX, debugY + 105, 0, "player.alive = true");
		else
			app->fonts->BlitText(debugX, debugY + 105, 0, "player.alive = false");

		//Scene Fight
		for (size_t i = 0; i < app->sceneFight->turnList.Count(); i++)
		{
			PartyMember* member = app->sceneFight->turnList.At(i)->data;

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

		if (app->sceneFight->turnMember != nullptr)
		{
			app->fonts->BlitText(debugX - 250, debugY, 0, app->sceneFight->turnMember->name.GetString());
		}


		//Scale
		std::string currentScale = app->scaleObj->ScaleTypeToString(app->scaleObj->GetCurrentScale());
		if (!app->scene->player->dead)
			app->fonts->BlitText(debugX, debugY + 200, 0, currentScale.c_str());
		else
			app->fonts->BlitText(debugX, debugY + 200, 0, currentScale.c_str());
		
	}

	//Camera limits
	if (camLimits)
	{
		/*
		int scale = app->win->GetScale();

		app->scene->rectCamera.x = app->render->camera.w * 0.4;
		app->scene->rectCamera.y = app->render->camera.h * 0.4;
		app->scene->rectCamera.w = app->render->camera.w * 0.2;
		app->scene->rectCamera.h = app->render->camera.h * 0.2;
		app->render->DrawRectangle(app->scene->rectCamera, 0, 255, 0, 255, false, false);
		*/
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
	b2World* world = app->physics->GetWorld();

	int scale = app->scaleObj->ScaleTypeToInt(app->scaleObj->GetCurrentScale());

	// Iterate all objects in the world and draw shapes
	// You need to provide your own macro to translate meters to pixels
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				uint width, height;
				app->win->GetWindowSize(width, height);
				b2Vec2 pos = f->GetBody()->GetPosition();
				app->render->DrawCircle(METERS_TO_PIXELS(pos.x) * scale,
					METERS_TO_PIXELS(pos.y) * scale,
					METERS_TO_PIXELS(shape->m_radius) * scale, 255, 255, 255);
			}
			break;

			// Draw polygons ------------------------------------------------
			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
				int32 count = polygonShape->GetVertexCount();
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->GetVertex(i));
					if (i > 0)
						app->render->DrawLine(METERS_TO_PIXELS(prev.x) * scale, METERS_TO_PIXELS(prev.y) * scale, METERS_TO_PIXELS(v.x) * scale, METERS_TO_PIXELS(v.y) * scale, 255, 255, 100);

					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->GetVertex(0));
				app->render->DrawLine(METERS_TO_PIXELS(prev.x) * scale, METERS_TO_PIXELS(prev.y) * scale, METERS_TO_PIXELS(v.x) * scale, METERS_TO_PIXELS(v.y) * scale, 255, 100, 100);
			}
			break;

			// Draw chains contour -------------------------------------------
			case b2Shape::e_chain:
			{
				b2ChainShape* shape = (b2ChainShape*)f->GetShape();
				b2Vec2 prev, v;

				for (int32 i = 0; i < shape->m_count; ++i)
				{
					v = b->GetWorldPoint(shape->m_vertices[i]);
					if (i > 0)
						app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
					prev = v;
				}

				v = b->GetWorldPoint(shape->m_vertices[0]);
				app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
			}
			break;

			// Draw a single segment(edge) ----------------------------------
			case b2Shape::e_edge:
			{
				b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
				b2Vec2 v1, v2;

				v1 = b->GetWorldPoint(shape->m_vertex0);
				v1 = b->GetWorldPoint(shape->m_vertex1);
				app->render->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
			}
			break;
			}

			// TODO 1: If mouse button 1 is pressed ...
			// app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN
			// test if the current body contains mouse position
		}
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
		case EntityType::ENEMY:			color = Red;		break;
		case EntityType::FLYING_ENEMY:	color = Red;		break;
		case EntityType::ITEM:			color = Green;		break;
		case EntityType::COIN:			color = Yellow;		break;
		default:						color = White;		break;
		}

		SDL_Rect rect = { pEntity->position.x, pEntity->position.y, 32, 32};
		app->render->DrawRectangle(rect, color.r, color.g, color.b, 255, false);
		app->render->DrawRectangle(rect, color.r, color.g, color.b, 64, true);
	}
}