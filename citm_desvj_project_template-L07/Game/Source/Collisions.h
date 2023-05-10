#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

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

struct Body
{
	ColliderShape shape;
	ColliderType type;
	MapZone mapZone;
	dPoint pos;
	dPoint vel;
	double w, h;
	double r;
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
	bool CheckCollision(const Body& body1, const Body& body2);
	void SolveCollision(Body* body1, Body* body2);

	void RectRectCollision(Body* body1, Body* body2);
	void CirCirCollision(Body* body1, Body* body2);
	void CirRectCollision(Body* body1, Body* body2);

	void DestroyMapColliders();

private:

};

#endif //__COLLISIONS_H__