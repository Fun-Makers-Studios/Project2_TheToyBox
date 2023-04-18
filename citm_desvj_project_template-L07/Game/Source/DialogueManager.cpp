#include "DialogueManager.h"
#include "Textures.h"
#include "Render.h"
#include "Fonts.h"
#include "Input.h"
#include "App.h"
#include "Scene.h"
#include "Fonts.h"

#include "Log.h"

DialogueManager::DialogueManager(Scene* scene) {
	LOG("Loading Dialogue Manager");

	this->scene = scene;

	file = app->configNode.child("dialoguemanager");
	dialoguetext = app->tex->Load((const char*)file.attribute("dialoguetext").as_string());
	dialogueFontId = app->fonts->Load((const char*)file.attribute("font").as_string(), (const char*)file.attribute("lookuptable").as_string(), file.attribute("rows").as_int());

	dialogueLoaded = false;
}

DialogueManager::~DialogueManager(){
	
}

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
	else if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		currentDialogue->currentNode = currentDialogue->SetCurrentNode(currentDialogue->currentNode->currentOption->nextNodeId);
		if (currentDialogue->currentNode == nullptr) {
			this->Unload();
		}
	}
}

void DialogueManager::Draw() {
	SDL_Rect rect = {0,0,1182,196};
	app->render->DrawTexture(dialoguetext, 49 + app->render->camera.x, 505 + app->render->camera.y, &rect);

	int lines = app->fonts->BlitText2(95, 540, dialogueFontId, (const char*)this->currentDialogue->currentNode->text.GetString(), 8, 1090);

	iPoint displacement = { 230, ((lines * (int)app->fonts->fonts[dialogueFontId].char_h) + 540) + 16 };
	ListItem<Option*>* op = this->currentDialogue->currentNode->options.start;
	while (op != nullptr)
	{
		if (currentDialogue->currentNode->currentOption->id == op->data->id) {
			rect = { 1182,0,15,21 };
			app->render->DrawTexture(dialoguetext, 175 + app->render->camera.x, displacement.y + app->render->camera.y, &rect);
		}

		displacement.y += app->fonts->fonts[dialogueFontId].char_h * app->fonts->BlitText2(displacement.x, displacement.y, dialogueFontId, (const char*)op->data->text.GetString(), 8, 890);
		op = op->next;
	}
}

void DialogueManager::Unload() {
	LOG("Unloading Dialog");

	currentDialogue->nodes.Clear();
	RELEASE(currentDialogue);

	dialogueLoaded = false;
	app->scene->gamePaused = false;
}

void DialogueManager::CleanUp() {
	app->fonts->UnLoad(dialogueFontId);
	app->tex->UnLoad(dialoguetext);
}