#ifndef __DIALOGUEMANAGER_H__
#define __DIALOGUEMANAGER_H__

#include "Dialogue.h"
#include "List.h"

#include "SDL/include/SDL.h"
#include "PugiXml/src/pugixml.hpp"

class Scene;

class DialogueManager
{
public:
	DialogueManager(Scene* scene);
	~DialogueManager();

	void Load(int dialogueId);
	void Update();
	void Draw();
	void Unload();

	bool dialogueLoaded;

private:
	Scene* scene;
	pugi::xml_node file;

	Dialogue* currentDialogue;
	SDL_Texture* dialoguetext;

	uint dialogueFontId;
};

#endif // __DIALOGUEMANAGER_H__