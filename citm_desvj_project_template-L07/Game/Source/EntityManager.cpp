#include "EntityManager.h"
#include "Player.h"
#include "Item.h"
#include "KidEnemy.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{

}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start()
{
	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type, pugi::xml_node itemNode)
{
	Entity* entity = nullptr; 

	switch (type)
	{

	case EntityType::PLAYER:
		entity = new Player(itemNode);
		break;
	
	case EntityType::NPC:
		entity = new NPC(itemNode);
		break;
	
	case EntityType::ENEMY_KID:
		entity = new KidEnemy(itemNode);
		break;

	case EntityType::ITEM:
		entity = new Item(itemNode);
		break;

	default: break;
	}

	// Created entities are added to the list
	AddEntity(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::PreUpdate()
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->PreUpdate();
	}

	return ret;
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	// Destroy entities
	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->needToDestroy)
		{
			item->data->CleanUp();
			DestroyEntity(pEntity);
		}
	}

	// Update entities
	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active)
			ret = item->data->Update();
	}

	//cSort Entites
	SortEntitiesZ();

	return ret;
}

bool EntityManager::PostUpdate()
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->PostUpdate();
	}

	// HEKATE - Should find a better place for this
	// Draw top layer
	app->map->Draw(true);

	return ret;
}

// HEKATE - Sort depending on y center? Should sort pickable Items?
void EntityManager::SortEntitiesZ()
{
	bool swapped = true;

	while (swapped)
	{
		swapped = false;
		ListItem<Entity*>* item;
		Entity* pEntity = NULL;

		for (item = entities.start; item != NULL && item->next != NULL; item = item->next)
		{
			if (item->data->body->pos.y > item->next->data->body->pos.y)
			{
				SWAP(item->data, item->next->data);
				swapped = true;
			}
		}
	}
}