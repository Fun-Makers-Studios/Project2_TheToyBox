#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Point.h"
#include "NPC.h"
#include "Item.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(bool startEnabled);

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:

	Player* player;
	//List<Chest*> rings;

	const char* musicPath;

	Animation animation;

	bool debug;

	bool questMenu;

	List<NPC*>npcs;
	List<Item*>items;

	int points;

private:
	SDL_Texture* img;

	int font_text;
	char pointsText[6] = { "\0" };
};

#endif // __SCENE_H__