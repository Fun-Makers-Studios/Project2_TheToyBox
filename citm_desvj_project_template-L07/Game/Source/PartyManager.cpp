#include "PartyManager.h"

PartyManager::PartyManager(){}

PartyManager::~PartyManager(){}

void PartyManager::AddMemberToParty(PartyMember* member)
{
	party.Add(member);
}