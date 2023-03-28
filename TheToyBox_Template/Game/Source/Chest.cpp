#include "Chest.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Log.h"
#include "Point.h"

Chest::Chest(int posX, int posY, const char* textPath) : Item(posX, posY, textPath)
{
	itemType = ItemType::RING;
}

Chest::~Chest() {}

bool Chest::Update() {
	return true;
}

void Chest::Draw() {
	app->render->DrawTexture(texture, tile.x * 32, tile.y * 32);
}