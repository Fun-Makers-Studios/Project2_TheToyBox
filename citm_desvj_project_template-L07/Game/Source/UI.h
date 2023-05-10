#ifndef __UI_H__
#define __UI_H__

#include "Module.h"
#include "Fonts.h"
#include "SDL_mixer/include/SDL_mixer.h"


struct SDL_Texture;

class UI : public Module
{
public:
	UI();

	// Destructor
	virtual ~UI();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

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


	void BlitPartyStats();

	uint font1_id = -1;
	uint font2_id = -1;
	uint font2Red_id = -1;
	uint font3_id = -1;

	SDL_Texture* livesTex = nullptr;
	const char* livesTexPath;
	
	SDL_Texture* coinsTex = nullptr;
	const char* coinsTexPath;

private:

	const char* font1Path;
	const char* font2Path;
	const char* font2_RedPath;
	const char* font3Path;

};

#endif // __UI_H__