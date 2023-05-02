#ifndef __SCENE_H__
#define __SCENE_H__

#include "NPC.h"
#include "Item.h"

class Scene
{
public:

	Scene() {}

	virtual ~Scene() {}

	// Called when scene initially created. Called once.
	virtual bool Awake(pugi::xml_node& config) = 0;

	// Called whenever a scene is transitioned into. Can be 
	// called many times in a typical game cycle.
	virtual bool Start() {};

	// Called before all Updates
	virtual bool PreUpdate() {};

	// Called each loop iteration
	virtual bool Update(float dt) {};

	// Called before all Updates
	virtual bool PostUpdate() {};

	// Called before quitting
	virtual bool CleanUp() {};

	// Returns the scene's ID
	virtual SString GetID() { return id; }

	// Sets the scene's ID
	virtual void SetID(SString id) { this->id = id; }

private:

	SString id;
	List<NPC*>npcs;
	List<Item*>items;
};

#endif // __SCENE_H__