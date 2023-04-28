#pragma once
#include "Module.h"
#include "Entity.h"

enum class ColliderShape
{
	CIRCLE,
	RECTANGLE
};

enum class ColliderType
{
	PLAYER,
	NPC,
	ITEM,
	ENEMY,
	PLATFORM,
	WALL,
	WATER,
	WIN_ZONE,
	CHECKPOINT,
	UNKNOWN
};

struct Body
{
	ColliderShape shape;
	ColliderType type;
	dPoint pos;
	dPoint vel;
	double r;
};

enum class MapZone
{
	PLAYER,

	HOUSE1_TO_TOWN,
	TOWN_TO_HOUSE1,
	
	HOUSEBASE_TO_HOUSEFLOOR,
	HOUSEFLOOR_TO_HOUSEFBASE,
	
	TAVERN_TO_TOWN,
	TOWN_TO_TAVERN,
	
	TOWN_TO_INN,
	INN_TO_TOWN,

	UNKNOWN
};

class Collisions : public Module
{
public:
	Collisions();
	~Collisions();

	bool Awake(){ return true; }
	bool Start();
	bool PreUpdate();
	bool Update(double dt);
	bool PostUpdate();
	bool CleanUp();

	//Collision Solver
	bool CheckCollision(Body* a, Body* b);

	void DestroyMapColliders();

private:

};