#pragma once

#include "SString.h"
#include "List.h"
#include "App.h"

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

class PartyMember
{
public:
	PartyMember(SString name, uint maxHp, uint maxMana, uint attack, uint defense, uint speed, uint crit, SDL_Texture* texture)
	{
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
	};

	~PartyMember() {};

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
};

class PartyManager
{
public:
	PartyManager();

	~PartyManager();

	void AddMemberToParty(PartyMember* member);

private:
	List<PartyMember*> party;

};