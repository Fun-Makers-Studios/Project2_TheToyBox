#include "PartyManager.h"
#include "Textures.h"

PartyManager::PartyManager(){}

PartyManager::~PartyManager(){}

bool PartyManager::Start()
{
	//Create party
	uchar partyCount = 0;
	for (pugi::xml_node itemNode = app->configNode.child("sceneFight").child("partymember"); itemNode; itemNode = itemNode.next_sibling("partymember"))
	{
		//type
		SString typeStr = itemNode.attribute("type").as_string();
		MemberType type = typeStr == "ally" ? MemberType::ALLY : MemberType::ENEMY;

		//texture
		const char* path = itemNode.attribute("texturepath").as_string();
		SDL_Texture* tex = app->tex->Load(path);

		SDL_Rect textureRect;
		SString nameStr = itemNode.attribute("name").as_string();
		if (nameStr == "zero")
			textureRect = { 32, 0, 32, 64 };
		else if (nameStr == "sophie")
			textureRect = { 128, 0, 32, 64 };
		

		//battle position
		int offsetX = 350;
		int offsetY = 300;

		iPoint position;
		position.x = offsetX - 32 * partyCount;;
		position.y = offsetY + 96 * partyCount;
		partyCount++;

		PartyMember* member = new PartyMember(
			type,
			MemberStatus::NORMAL,
			itemNode.attribute("name").as_string(),
			itemNode.attribute("maxHp").as_uint(),
			itemNode.attribute("maxMana").as_uint(),
			itemNode.attribute("level").as_uint(),
			itemNode.attribute("attack").as_uint(),
			itemNode.attribute("defense").as_uint(),
			itemNode.attribute("speed").as_uint(),
			itemNode.attribute("critRate").as_uint(),
			tex,
			position,
			textureRect);

		app->partyManager->AddMemberToParty(member);
	}

	return true;

}

void PartyManager::AddMemberToParty(PartyMember* member)
{
	party.Add(member);
}