#pragma once

#include "SString.h"
#include "List.h"

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

struct PartyMember
{
	SString name;

	uint maxHp;
	uint currentHp;
	uint maxMana;
	uint currentMana;
	uint attack;
	uint defense;
	uint speed;
	uint crit;
	uint critRate;

	SDL_Texture* texture;
};

class PartyManager
{
public:
	PartyManager();

	~PartyManager();

	void AddMemberToParty(PartyMember*);

private:
	List<PartyMember*> party;

};