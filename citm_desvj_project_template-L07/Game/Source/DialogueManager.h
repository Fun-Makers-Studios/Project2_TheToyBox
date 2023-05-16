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
	bool Update();
	void Draw();
	void Unload();
	void CleanUp();
	Dialogue* GetCurrentDialogue() { return currentDialogue; }

	bool dialogueLoaded;

	uint dialogueFontId;

private:
	Scene* scene;
	pugi::xml_node file;

	Dialogue* currentDialogue;
	SDL_Texture* dialoguetext;
};

#endif // __DIALOGUEMANAGER_H__