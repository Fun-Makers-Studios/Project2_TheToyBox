#ifndef __SCENEBATTLE_H__
#define __SCENEBATTLE_H__

#include "Animation.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Player.h"
#include "NPC.h"
#include "Item.h"

struct SDL_Texture;

class SceneBattle : public Scene
{
public:

	SceneBattle();
	virtual ~SceneBattle();

	void OnCreate(pugi::xml_node&) override;
	void OnDestroy() override;

	void OnActivate();
	void OnDeactivate();

	void PreUpdate();
	void Update(float dt);
	void PostUpdate();

	void CleanUp();

	virtual SString GetID() { return id; }
	virtual void SetID(SString id) { this->id = id; }

private:

	SString id;
	bool debug;

	const char* musicPath;
	const char* logoPath;

	SDL_Texture* logoImg;

	int font_text;
	char pointsText[6] = { "\0" };
};

#endif // __SCENEBATTLE_H__
