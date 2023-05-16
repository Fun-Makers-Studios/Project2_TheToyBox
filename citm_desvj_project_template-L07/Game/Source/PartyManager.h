#pragma once

#include "SString.h"
#include "List.h"
#include "App.h"
#include "Point.h"
#include "Item.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

enum class GearType
{
	HEAD,
	CHEST,
	PANTS,
	WEAPON
};

struct Gear
{
	Item* helmet = nullptr;
	Item* chest = nullptr;
	Item* pants = nullptr;
	Item* weapon = nullptr;
};

enum class StatType
{
	HP,
	MANA,
	ATTACK,
	DEFENSE,
	SPEED,
	CRIT,
	CRIT_RATE
};

struct Stats
{

};

enum class MemberType
{
	ALLY,
	ENEMY
};

enum class MemberStatus
{
	NORMAL,
	POISONED,
	STUNED,
	DEAD
};

class PartyMember
{
public:
	PartyMember(MemberType type, MemberStatus status, SString name, uint maxHp, uint maxMana, uint level, uint attack, uint defense, uint speed, uint critRate, iPoint initPos, SDL_Texture* texture, SDL_Rect textureRect)
	{
		this->type = type;
		this->status = status;

		this->name = name;
		this->level = level;

		this->maxHp = maxHp;
		this->maxMana = maxMana;
		this->attack = attack;
		this->defense = defense;
		this->speed = speed;
		this->critRate = critRate;

		this->currentHp = maxHp;
		this->currentMana = maxMana;

		this->initPos = initPos;
		this->pos = initPos;
		this->texture = texture;
		this->textureRect = textureRect;
	};

	~PartyMember() {};

	MemberType type;
	Gear gear;
	MemberStatus status;
	SString name;

	uint maxHp;
	uint level;
	uint currentHp;
	uint maxMana;
	uint currentMana;
	uint attack;
	uint defense;
	uint speed;
	uint critRate;

	SDL_Texture* texture;
	SDL_Rect textureRect;

	iPoint initPos;
	iPoint pos;
};

class PartyManager: public Module
{
public:
	PartyManager();

	~PartyManager();

	bool Start();

	bool LoadPartyMember(SString name);
	void AddMemberToParty(PartyMember* member);

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

public:
	List<PartyMember*> party;
	const char* enemyToFight = nullptr;
	uint maxPartyMembers = 3;
	uchar partyCount = 0;
};