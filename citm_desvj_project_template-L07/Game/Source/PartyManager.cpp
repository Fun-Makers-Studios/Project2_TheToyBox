#include "PartyManager.h"
#include "Textures.h"

PartyManager::PartyManager() : Module() 
{
	name.Create("partymanager");
}

PartyManager::~PartyManager(){}

bool PartyManager::Start()
{
	// Scale
	int scale = app->scaleObj->ScaleTypeToInt(app->scaleObj->GetCurrentScale());

	//Create party
	uchar partyCount = 0;

	for (pugi::xml_node itemNode = app->configNode.child("sceneFight").child("partymember"); itemNode; itemNode = itemNode.next_sibling("partymember"))
	{
		if (itemNode.attribute("initial").as_bool() == false) { continue; }

		//type
		SString typeStr = itemNode.attribute("type").as_string();
		MemberType type = typeStr == "ally" ? MemberType::ALLY : MemberType::ENEMY;

		//texture
		const char* path = itemNode.attribute("texturepath").as_string();
		SDL_Texture* tex = app->tex->Load(path);

		SDL_Rect textureRect;
		SString nameStr = itemNode.attribute("name").as_string();
		if (nameStr == "zero")
			textureRect = { 0, 23, 48, 57 };
		//textureRect = { 32, 0, 32, 64 };
		else if (nameStr == "sophie")
			textureRect = { 49, 24, 44, 56 };
		//textureRect = { 128, 0, 32, 64 };
		
		//battle position
		int offsetX = 210;
		int offsetY = 50;

		iPoint position;
		position.x = (offsetX - 32 * partyCount) / scale;
		position.y = (offsetY + 96 * partyCount) / scale;

		//Create Member
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
			position, tex, textureRect);

		AddMemberToParty(member);

		partyCount++;
	}

	return true;

}

bool PartyManager::LoadPartyMember(SString name) {
	bool ret = false;
	pugi::xml_node itemNode;
	for (pugi::xml_node itemNode = app->configNode.child("sceneFight").child("partymember"); itemNode; itemNode = itemNode.next_sibling("partymember"))
	{
		if (itemNode.attribute("name").as_string() == name.GetString()) {
			ret = true;
			break;
		}
	}
	if (!ret) { return ret; }

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
		position,
		tex,
		textureRect);

	AddMemberToParty(member);

	return ret;
}

void PartyManager::AddMemberToParty(PartyMember* member)
{
	party.Add(member);
}

bool PartyManager::LoadState(pugi::xml_node& data)
{
	/*ListItem<PartyMember*>* pmemberItem;
	for (pmemberItem = party.start; pmemberItem != NULL; pmemberItem = pmemberItem->next)
	{
		pugi::xml_node partyMember = data.append_child("partymember");
		pmemberItem->data->name = data.child("partymember").attribute("name").as_string();
		pmemberItem->data->status = (MemberStatus)data.child("partymember").attribute("status").as_uint();
		pmemberItem->data->maxHp = data.child("partymember").attribute("maxHp").as_uint();
		pmemberItem->data->maxMana = data.child("partymember").attribute("maxMana").as_uint();
		pmemberItem->data->currentHp = data.child("partymember").attribute("currentHp").as_uint();
		pmemberItem->data->currentMana = data.child("partymember").attribute("currentHp").as_uint();
		pmemberItem->data->level = data.child("partymember").attribute("level").as_uint();
		pmemberItem->data->attack = data.child("partymember").attribute("attack").as_uint();
		pmemberItem->data->defense = data.child("partymember").attribute("defense").as_uint();
		pmemberItem->data->speed = data.child("partymember").attribute("speed").as_uint();
		pmemberItem->data->critRate = data.child("partymember").attribute("critRate").as_uint();
		pmemberItem->data->initPos.x = data.child("partymember").attribute("fightPosX").as_int();
		pmemberItem->data->initPos.y = data.child("partymember").attribute("fightPosY").as_int();
	}*/

	return true;
}

bool PartyManager::SaveState(pugi::xml_node& data)
{
	ListItem<PartyMember*>* pmemberItem;

	for (pmemberItem = party.start; pmemberItem != NULL; pmemberItem = pmemberItem->next)
	{
		pugi::xml_node partyMember = data.append_child("partymember");
		partyMember.append_attribute("name") = pmemberItem->data->name.GetString();
		partyMember.append_attribute("status") = (uint)pmemberItem->data->status;
		partyMember.append_attribute("maxHP") = pmemberItem->data->maxHp;
		partyMember.append_attribute("maxMana") = pmemberItem->data->maxMana;
		partyMember.append_attribute("currentHp") = pmemberItem->data->currentHp;
		partyMember.append_attribute("currentMana") = pmemberItem->data->currentMana;
		partyMember.append_attribute("level") = pmemberItem->data->level;
		partyMember.append_attribute("attack") = pmemberItem->data->attack;
		partyMember.append_attribute("defense") = pmemberItem->data->defense;
		partyMember.append_attribute("speed") = pmemberItem->data->speed;
		partyMember.append_attribute("critRate") = pmemberItem->data->critRate;
		partyMember.append_attribute("fightPosX") = pmemberItem->data->initPos.x;
		partyMember.append_attribute("fightPosY") = pmemberItem->data->initPos.y;
	}

	return true;
}