#pragma once

#include "SString.h"
#include "List.h"
#include "App.h"
#include "Point.h"

struct SDL_Texture;

enum class GearType
{
	HEAD,
	CHEST,
	PANTS,
	WEAPON,
	RING
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

struct Gear
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
	PartyMember(MemberType type, MemberStatus status, SString name, uint maxHp, uint maxMana, uint attack, uint defense, uint speed, uint critRate, SDL_Texture* texture, iPoint fightPosition)
	{
		this->type = type;
		this->status = status;
		this->name = name;
		this->maxHp = maxHp;
		this->currentHp = maxHp;
		this->maxMana = maxMana;
		this->currentMana = maxMana;
		this->attack = attack;
		this->defense = defense;
		this->speed = speed;
		this->critRate = critRate;
		this->texture = texture;
		this->fightPosition = fightPosition;
	};

	~PartyMember() {};

	MemberType type;
	MemberStatus status;
	SString name;

	uint maxHp;
	uint currentHp;
	uint maxMana;
	uint currentMana;
	uint attack;
	uint defense;
	uint speed;
	uint critRate;

	SDL_Texture* texture;

	iPoint fightPosition;
};

class PartyManager: public Module
{
public:
	PartyManager();

	~PartyManager();

	void AddMemberToParty(PartyMember* member);

public:
	List<PartyMember*> party;
	const char* enemyToFight;

};