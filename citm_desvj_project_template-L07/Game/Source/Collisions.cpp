#include "Collisions.h"
#include "Input.h"
#include "app.h"
#include "Log.h"
#include "math.h"
#include "SDL/include/SDL_keycode.h"
#include "Defs.h"
#include "Log.h"
#include "Render.h"
#include "Player.h"
#include "Window.h"
#include "SceneManager.h"
#include "Map.h"
#include "Pathfinding.h"
#include "EntityManager.h"
#include "MenuManager.h"
#include "Debug.h"
#include "Audio.h"
#include <cmath>

Collisions::Collisions() : Module()
{
    name.Create("collisions");
}

// Destructor
Collisions::~Collisions()
{
}


bool Collisions::Start()
{
	LOG("Start Collisions module");


	return true;
}

// 
bool Collisions::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool Collisions::Update(double dt)
{
    bool ret = true;

    

    // Check player collision in a 3x3 tile area
    

    return ret;
}

// Draw colliders
bool Collisions::PostUpdate()
{
	bool ret = true;
  
    int tileH = app->map->mapData.tileHeight;
    int tileW = app->map->mapData.tileWidth;

    // MAP COLLISIONS
    Body playerBody = *app->sceneManager->sceneGame->player->body;
    iPoint tilePos = app->map->WorldToMap(playerBody.pos.x, playerBody.pos.y);

    for (int i = tilePos.x-1; i <= tilePos.x+1; i++) //rows
    {
        for (int j = tilePos.y-1; j <= tilePos.y+1; j++) //cols
        {
            if (app->pathfinding->GetTileAt({ i, j }) == 1) //1 non walkable tiles, 255 invalid walk code
            {
                SDL_Rect rect = { i * tileW, j * tileH, tileW, tileH };
                
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
                
                if (CheckCollision(playerBody, tileCollider))
                {                    
                    SolveCollision(app->sceneManager->sceneGame->player->body, &tileCollider);
                }
            }
        }
    }

    // ENTITY COLLISIONS
    ListItem<Entity*>* item;


    for (item = app->entityManager->entities.start; item != NULL; item = item->next)
    {
        if (CheckCollision(*app->sceneManager->sceneGame->player->body, *item->data->body))
        {
            SolveCollision(app->sceneManager->sceneGame->player->body, item->data->body);
        }
    }
    
    ListItem<Body*>* bodyItem;

    for (bodyItem = app->map->mapTeleports.start; bodyItem != NULL; bodyItem = bodyItem->next)
    {
        if (CheckCollision(*app->sceneManager->sceneGame->player->body, *bodyItem->data))
        {
            SolveCollision(app->sceneManager->sceneGame->player->body, bodyItem->data);
        }
    }

	return ret;
}


// Called before quitting
bool Collisions::CleanUp()
{
	LOG("Destroying physics world");

	DestroyMapColliders();

	return true;
}

void Collisions::DestroyMapColliders()
{
}


bool Collisions::CheckCollision(const Body& body1, const Body& body2)
{
    if (body1.shape == ColliderShape::CIRCLE && body2.shape == ColliderShape::CIRCLE)
    {
        // Check collision between two circles
        double distance = body1.pos.DistanceTo(body2.pos);
        double radiusSum = body1.r + body2.r;
        return (distance <= radiusSum);
    }
    else if (body1.shape == ColliderShape::RECTANGLE && body2.shape == ColliderShape::RECTANGLE)
    {
        // Check collision between two rectangles
        double xDist = abs(body1.pos.x - body2.pos.x);
        double yDist = abs(body1.pos.y - body2.pos.y);
        double xOverlap = (body1.w + body2.w) / 2 - xDist;
        double yOverlap = (body1.h + body2.h) / 2 - yDist;
        return (xOverlap > 0 && yOverlap > 0);
    }
    else
    {
        // Swap so that body1 is the circle
        Body circle, rectangle;
        if (body1.shape == ColliderShape::CIRCLE) {
            circle = body1;
            rectangle = body2;
        }
        else {
            circle = body2;
            rectangle = body1;
        }

        // Calculate distance between circle center and rectangle center
        double circleDistanceX = std::abs(circle.pos.x - rectangle.pos.x);
        double circleDistanceY = std::abs(circle.pos.y - rectangle.pos.y);

        // Check if circle is too far away from rectangle to collide
        if (circleDistanceX > (rectangle.w / 2.0 + circle.r)) { return false; } 
        if (circleDistanceY > (rectangle.h / 2.0 + circle.r)) { return false; }

        // Check if circle is close enough to collide with rectangle
        if (circleDistanceX <= (rectangle.w / 2.0)) { return true; }
        if (circleDistanceY <= (rectangle.h / 2.0)) { return true; }

        // Check if circle collides with corner of rectangle
        double cornerDistanceSq = pow(circleDistanceX - rectangle.w / 2.0, 2) + pow(circleDistanceY - rectangle.h / 2.0, 2);
        return (cornerDistanceSq <= pow(circle.r, 2));
    }
}

void Collisions::SolveCollision(Body* body1, Body* body2)
{
    if (app->debug->godMode) { return; }

    switch (body2->type)
    {
        case ColliderType::WALL:
            CirRectCollision(body1, body2);
            break;
         
        case ColliderType::NPC:
            CirCirCollision(body1, body2);
            break;

        case ColliderType::ENEMY:
            CirCirCollision(body1, body2);
            app->sceneManager->sceneGame->FightKid();
            break; 

        case ColliderType::ITEM:

            ListItem<Item*>* itemsItem;
            for (itemsItem = app->sceneManager->sceneGame->itemsList.start; itemsItem != NULL; itemsItem = itemsItem->next)
            {
                if (itemsItem->data->body == body2)
                {
                    itemsItem->data->takeItem = true;
                }
            }

            break;

        case ColliderType::BUTTON:
            CirCirCollision(body1, body2);
            break;
        
        case ColliderType::MOVABLEOBJ:
            
            break;

        case ColliderType::UNKNOWN:
            break;

        default:
            break;
    }
    
    switch (body2->mapZone)
    {

    case MapZone::HOUSE1_TO_TOWN:
        LOG("GO TO TOWN");
        app->sceneManager->sceneGame->mapName = "town";
        app->sceneManager->sceneGame->player->newPos = { 192, 256 };
        app->sceneManager->sceneGame->isMapChanging = true;
        scaleMap = ScaleType::WORLD;
        app->audio->PlayMusic("Assets/Audio/Music/town.ogg", 1.0F);
        break;

    case MapZone::TOWN_TO_HOUSE1:
        LOG("GO TO HOUSE BASEMENT");
        app->sceneManager->sceneGame->mapName = "housebasement";
        app->sceneManager->sceneGame->player->newPos = { 640, 480 };
        app->sceneManager->sceneGame->isMapChanging = true;
        scaleMap = ScaleType::HOUSE;
        app->audio->PlayMusic("Assets/Audio/Music/house.ogg", 1.0F);
        break;

    case MapZone::HOUSEBASE_TO_HOUSEFLOOR:
        LOG("GO TO HOUSE FLOOR");
        app->sceneManager->sceneGame->mapName = "housefloor";
        app->sceneManager->sceneGame->player->newPos = { 840, 140};
        app->sceneManager->sceneGame->isMapChanging = true;
        scaleMap = ScaleType::HOUSE;
        app->audio->PlayMusic("Assets/Audio/Music/house.ogg", 1.0F);
        break;

    case MapZone::HOUSEFLOOR_TO_HOUSEFBASE:
        LOG("GO TO HOUSE BASEMENT");
        app->sceneManager->sceneGame->mapName = "housebasement";
        app->sceneManager->sceneGame->player->newPos = { 660, 150 };
        app->sceneManager->sceneGame->isMapChanging = true;
        scaleMap = ScaleType::HOUSE;
        app->audio->PlayMusic("Assets/Audio/Music/house.ogg", 1.0F);
        break;

    case MapZone::TOWN_TO_TAVERN:
        LOG("GO TO TAVERN");
        app->sceneManager->sceneGame->mapName = "tavern";
        app->sceneManager->sceneGame->player->newPos = { 640, 500 };
        app->sceneManager->sceneGame->isMapChanging = true;
        scaleMap = ScaleType::HOUSE;
        app->audio->PlayMusic("Assets/Audio/Music/house.ogg", 1.0F);
        break;

    case MapZone::TAVERN_TO_TOWN:
        LOG("GO TO TOWN");
        app->sceneManager->sceneGame->mapName = "town";
        app->sceneManager->sceneGame->player->newPos = { 800, 352 };
        app->sceneManager->sceneGame->isMapChanging = true;
        scaleMap = ScaleType::WORLD;
        app->audio->PlayMusic("Assets/Audio/Music/town.ogg", 1.0F);
        break;

    case MapZone::TOWN_TO_INN:
        LOG("GO TO INN");
        app->sceneManager->sceneGame->mapName = "inn";
        app->sceneManager->sceneGame->player->newPos = { 960, 544 };
        app->sceneManager->sceneGame->isMapChanging = true;
        scaleMap = ScaleType::HOUSE;
        app->audio->PlayMusic("Assets/Audio/Music/house.ogg", 1.0F);
        break;

    case MapZone::INN_TO_TOWN:
        LOG("GO TO TOWN");
        app->sceneManager->sceneGame->mapName = "town";
        app->sceneManager->sceneGame->player->newPos = { 1184, 448 };
        app->sceneManager->sceneGame->isMapChanging = true;
        scaleMap = ScaleType::WORLD;
        app->audio->PlayMusic("Assets/Audio/Music/town.ogg", 1.0F);

        break;
    
    case MapZone::TOWN_TO_CIRCUS:
    {
        LOG("GO TO CIRCUS");

        SString str = "Circus ticket";

        for (ListItem<InventorySlot*>* invItem = app->menuManager->menuParty->inventorySlotList.start; invItem != nullptr; invItem = invItem->next)
        {
            if (invItem->data->item != nullptr && invItem->data->item->itemData.name == str)
            {
                app->sceneManager->sceneGame->mapName = "circusOne";
                app->sceneManager->sceneGame->player->newPos = { 160, 416 };
                app->sceneManager->sceneGame->isMapChanging = true;
                scaleMap = ScaleType::WORLD;
            }
        }
        app->audio->PlayMusic("Assets/Audio/Music/dungeon.ogg", 1.0F);

    }break;

    case MapZone::CIRCUS_TO_TOWN:
        LOG("GO TO TOWN");

        app->sceneManager->sceneGame->mapName = "town";
        app->sceneManager->sceneGame->player->newPos = { 1450, 96 };
        app->sceneManager->sceneGame->isMapChanging = true;
        scaleMap = ScaleType::WORLD;
        app->audio->PlayMusic("Assets/Audio/Music/town.ogg", 1.0F);

        break;
    
    case MapZone::CIRCUSONE_TO_CIRCUSTWO:
        LOG("GO TO CIRCUS 2");
        app->sceneManager->sceneGame->mapName = "circusTwo";
        app->sceneManager->sceneGame->player->newPos = { 256, 1344 };
        app->sceneManager->sceneGame->isMapChanging = true;
        scaleMap = ScaleType::WORLD;
        app->audio->PlayMusic("Assets/Audio/Music/far.ogg", 1.0F);

        break;
    
    case MapZone::CIRCUSTWO_TO_CIRCUSONE:
        LOG("GO TO CIRCUS");
        app->sceneManager->sceneGame->mapName = "circusOne";
        app->sceneManager->sceneGame->player->newPos = { 192, 1216 };
        app->sceneManager->sceneGame->isMapChanging = true;
        scaleMap = ScaleType::WORLD;
        app->audio->PlayMusic("Assets/Audio/Music/dungeon.ogg", 1.0F);

        break;

    }
}

void Collisions::RectRectCollision(Body* body1, Body* body2)
{
    // Currently not needed
}

void Collisions::CirCirCollision(Body* body1, Body* body2)
{
    // Vector from player circle center to NPC circle center
    dPoint collisionVector = body1->pos - body2->pos;

    // Distance between the centers of the two circles
    double distance = body1->pos.DistanceTo(body2->pos);

    // Sum of the radii of the two circles
    double totalRadius = body1->r + body2->r;

    // Calculate mtv if the is overlap
    if (distance <= totalRadius)
    {
        dPoint mtv = collisionVector.Normalize();
        mtv.x *= totalRadius - distance;
        mtv.y *= totalRadius - distance;
        body1->pos += mtv;
    }
}

void Collisions::CirRectCollision(Body* body1, Body* body2)
{
    // Closest point on the rectangle to the circle center
    dPoint topLeft = { body2->pos.x - body2->w / 2, body2->pos.y - body2->h / 2 };
    dPoint botRight = { body2->pos.x + body2->w / 2, body2->pos.y + body2->h / 2 };
    dPoint closestPoint = body1->pos.Clamp(topLeft, botRight);

    // Vector from the closest point on the rectangle to the circle center
    dPoint rectToCircle = body1->pos - closestPoint;

    // Distance between the closest point and the circle center
    double distance = body1->pos.DistanceTo(closestPoint);

    // Overlap distance between the player and the obstacle
    double overlapDistance = body1->r - distance;

    // Calculate mtv if there is overlap
    if (overlapDistance > 0.0)
    {
        dPoint mtv = rectToCircle.Normalize();
        mtv.x *= overlapDistance;
        mtv.y *= overlapDistance;

        body1->pos += mtv;
    }
}