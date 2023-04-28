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
#include "Scene.h"
#include "Map.h"
#include "Pathfinding.h"

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

    iPoint pos = app->map->WorldToMap(app->scene->player->body->pos.x, app->scene->player->body->pos.y);

    // Check player collision in a 3x3 tile area 
    for (int i = 0; i < pos.x; i++) //rows
    {
        for (int j = 0; j < pos.y; j++) //cols
        {
            if (i >= 0 && j >= 0) //valid map tiles
            {
                if (app->pathfinding->GetTileAt({ i, j }) == 1) //non walkable tiles
                {
                    Body tileCollider;
                    iPoint iPos = app->map->MapToWorld(i, j);
                    tileCollider.pos = { (double)iPos.x, (double)iPos.y };
                    tileCollider.shape = ColliderShape::RECTANGLE;

                    if (CheckCollision(app->scene->player->body, &tileCollider))
                    {

                    }
                }
            }
        }
    }


    return ret;
}

// Draw colliders
bool Collisions::PostUpdate()
{
	bool ret = true;

    int tileH = app->map->mapData.tileHeight;
    int tileW = app->map->mapData.tileWidth;

    for (int i = 0; i < app->map->mapData.width; i++) //rows
    {
        for (int j = 0; j < app->map->mapData.height; j++) //cols
        {
            if (app->pathfinding->GetTileAt(iPoint{ i, j }) == 1) //non walkable tiles, 255 invalid walk code
            {
                
                SDL_Rect rect = { i * tileW, j * tileH, tileW, tileH };
                app->render->DrawRectangle(rect, 255, 0, 0, 128);
                app->render->DrawRectangle(rect, 255, 0, 0, 255, false);
            }
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


bool Collisions::CheckCollision(Body* a, Body* b)
{
    // Calculate the distance between the two bodies
    double distance = std::sqrt(std::pow(a->pos.x - b->pos.x, 2) + std::pow(a->pos.y - b->pos.y, 2));

    if (a->shape == ColliderShape::CIRCLE && b->shape == ColliderShape::CIRCLE)
    {
        // If both bodies are circles, check if their radii overlap
        double radiusSum = a->r + b->r;
        return distance < radiusSum;
    }
    else if (a->shape == ColliderShape::RECTANGLE && b->shape == ColliderShape::RECTANGLE)
    {
        // If both bodies are rectangles, check if their bounding boxes overlap
        dPoint aMin = { a->pos.x - a->r, a->pos.y - a->r };
        dPoint aMax = { a->pos.x + a->r, a->pos.y + a->r };
        dPoint bMin = { b->pos.x - b->r, b->pos.y - b->r };
        dPoint bMax = { b->pos.x + b->r, b->pos.y + b->r };
        return (aMin.x < bMax.x&& aMax.x > bMin.x && aMin.y < bMax.y&& aMax.y > bMin.y);
    }
    else
    {
        //// If one body is a circle and the other is a rectangle, check if the circle and rectangle overlap
        //const Body& circle = a.isCircle ? a : b;
        //const Body& rect = a.isCircle ? b : a;

        //// Calculate the closest point on the rectangle to the circle
        //float closestX = std::max(rect.pos.x - rect.radius, std::min(circle.pos.x, rect.pos.x + rect.radius));
        //float closestY = std::max(rect.pos.y - rect.radius, std::min(circle.pos.y, rect.pos.y + rect.radius));

        //// Calculate the distance between the circle and the closest point on the rectangle
        //float distanceX = circle.pos.x - closestX;
        //float distanceY = circle.pos.y - closestY;
        //float distanceSquared = distanceX * distanceX + distanceY * distanceY;

        //// Check if the circle and rectangle overlap
        //return distanceSquared < (circle.radius* circle.radius);
    }
}