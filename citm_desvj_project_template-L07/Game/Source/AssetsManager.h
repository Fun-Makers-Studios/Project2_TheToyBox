#ifndef _ASSETSMANAGER_H_
#define _ASSETSMANAGER_H_

#include "Module.h"

#include "External/SDL/include/SDL.h"



class AssetsManager : public Module
{
public:

	AssetsManager();
	~AssetsManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Return the bytes of a PhysFS filehandle
	uint Load(const char* path, char** buffer) const;

	// Allows you to use pointers to memory instead of files or things such as images or samples
	SDL_RWops* Load(const char* path) const;

};

#endif // _ASSETSMANAGER_H_
