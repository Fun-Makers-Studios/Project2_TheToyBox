#ifndef __DIALOGUEMANAGER_H__
#define __DIALOGUEMANAGER_H__

#include "Dialogue.h"
#include "List.h"

#include "SDL/include/SDL.h"
#include "PugiXml/src/pugixml.hpp"

class SceneGame;

class DialogueManager
{
public:
	DialogueManager(SceneGame* scene);
	~DialogueManager();

	void Load(int dialogueId);
	void Update();
	void Draw();
	void Unload();
	void CleanUp();
	Dialogue* GetCurrentDialogue() { return currentDialogue; }

	bool dialogueLoaded;

	uint dialogueFontId;

private:
	SceneGame* scene;
	pugi::xml_node file;

	Dialogue* currentDialogue;
	SDL_Texture* dialoguetext;
};

#endif // __DIALOGUEMANAGER_H__