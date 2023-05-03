#pragma once
#include "Module.h"
#include "Entity.h"

#define PIXELS_PER_METER 32.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.03125f // this is 1 / PIXELS_PER_METER !
#define RAD_PER_DEG ((float)180/b2_pi)
#define DEG_PER_RAD ((float)b2_pi/180)

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)
#define RAD_TO_DEG(r) ((float) RAD_PER_DEG * r)
#define DEG_TO_RAD(r) ((float) DEG_PER_RAD * r)
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

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
	double w, h;
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
	bool CheckCollision(const Body& body1, const Body& body2);
	void SolveCollision(Body* body1, Body* body2);

	void RectRectCollision(Body* body1, Body* body2);
	void CirCirCollision(Body* body1, Body* body2);
	void CirRectCollision(Body* body1, Body* body2);

	void DestroyMapColliders();

private:

};