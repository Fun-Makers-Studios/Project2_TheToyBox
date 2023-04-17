#include "DialogueManager.h"
#include "Textures.h"
#include "Render.h"
#include "Fonts.h"
#include "Input.h"
#include "App.h"
#include "Scene.h"

#include "Log.h"

DialogueManager::DialogueManager(Scene* scene) {
	LOG("Loading Dialogue Manager");

	this->scene = scene;

	file = app->configNode.child("dialoguemanager");
	dialogtext = app->tex->Load((const char*)file.attribute("dialogtext").as_string());

	dialogueLoaded = false;
}

DialogueManager::~DialogueManager(){}

void DialogueManager::Load(int dialogueId) {
	LOG("Loading Dialog");

	for (pugi::xml_node node = file.child("dialogue"); node; node = node.next_sibling("dialogue"))
	{
		if (node.attribute("id").as_int() == dialogueId) {
			Dialogue* dialogue = new Dialogue(node);
			currentDialogue = dialogue;
		}
	}

	dialogueLoaded = true;
	app->scene->gamePaused = true;
}

void DialogueManager::Update() {
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
		Option* op = currentDialogue->currentNode->SetCurrentOption(currentDialogue->currentNode->currentOption->id - 1);
		if (op != nullptr) {
			currentDialogue->currentNode->currentOption = op;
		}
	}
	else if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
		Option* op = currentDialogue->currentNode->SetCurrentOption(currentDialogue->currentNode->currentOption->id + 1);
		if (op != nullptr) {
			currentDialogue->currentNode->currentOption = op;
		}
	}
}

void DialogueManager::Draw() {
	SDL_Rect rect = {0,0,1182,196};
	app->render->DrawTexture(dialogtext, 49, 505, &rect);

	rect = { 1182,0,15,21 };
	app->render->DrawTexture(dialogtext, 175, 622, &rect);

	//app->fonts->BlitText(672, 576, font_text, "POINTS", false);
}

void DialogueManager::Unload() {
	LOG("Unloading Dialog");

	currentDialogue->nodes.Clear();
	RELEASE(currentDialogue);

	dialogueLoaded = false;
	app->scene->gamePaused = false;
}