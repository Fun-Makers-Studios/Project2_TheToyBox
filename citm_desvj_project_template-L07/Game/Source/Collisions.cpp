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
#include "Debug.h"

#include <cmath>

Collisions::Collisions() : Module()
{
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

    // Draw map colliders
    for (int i = 0; i < app->map->mapData.width; i++) //rows
    {
        for (int j = 0; j < app->map->mapData.height; j++) //cols
        {
            if (app->pathfinding->GetTileAt({ i, j }) == 1) //non walkable tiles, 255 invalid walk code
            {
                SDL_Rect rect = { i * tileW, j * tileH, tileW, tileH };
                app->render->DrawRectangle(rect, 255, 255, 255, 128);
                app->render->DrawRectangle(rect, 255, 255, 255, 255, false);
            }
        }
    }

    // MAP COLLISIONS
    Body playerBody = *app->sceneManager->sceneGame->player->body;
    iPoint pos = app->map->WorldToMap(playerBody.pos.x, playerBody.pos.y);

    for (int i = pos.x-1; i <= pos.x+1; i++) //rows
    {
        for (int j = pos.y-1; j <= pos.y+1; j++) //cols
        {
            if (app->pathfinding->GetTileAt({ i, j }) == 1) //1 non walkable tiles, 255 invalid walk code
            {
                SDL_Rect rect = { i * tileW, j * tileH, tileW, tileH };

                app->render->DrawRectangle(rect, 255, 255, 0, 64);
                app->render->DrawRectangle(rect, 255, 255, 0, 196, false);

                Body tileCollider;
                tileCollider.type = ColliderType::WALL;
                tileCollider.shape = ColliderShape::RECTANGLE;
                iPoint iPos = app->map->MapToWorld(i, j);
                tileCollider.pos.x = iPos.x + tileW / 2;
                tileCollider.pos.y = iPos.y + tileH / 2;
                tileCollider.w = tileW;
                tileCollider.h = tileH;
                tileCollider.r = 16;
                
                if (CheckCollision(playerBody, tileCollider))
                {
                    app->render->DrawRectangle(rect, 255, 0, 0, 64);
                    app->render->DrawRectangle(rect, 255, 0, 0, 196, false);

                    SolveCollision(app->sceneManager->sceneGame->player->body, &tileCollider);
                }
            }
        }
    }

    // ENTITY COLLISIONS
    ListItem<Entity*>* item;
    Entity* pEntity = NULL;

    for (item = app->entityManager->entities.start; item != NULL; item = item->next)
    {
        if (CheckCollision(*app->sceneManager->sceneGame->player->body, *item->data->body))
        {
            SolveCollision(app->sceneManager->sceneGame->player->body, item->data->body);
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
            for (itemsItem = app->scene->itemsList.start; itemsItem != NULL; itemsItem = itemsItem->next)
            {
                if (itemsItem->data->body == body2)
                {
                    app->entityManager->DestroyEntity(itemsItem->data);
                }
            }

            break;

        case ColliderType::WIN_ZONE:
            break;

        case ColliderType::CHECKPOINT:
            break;

        case ColliderType::UNKNOWN:
            break;

        default:
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