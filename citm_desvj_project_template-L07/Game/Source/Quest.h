#ifndef __QUEST_H__
#define __QUEST_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

enum class QuestType
{
	TALK,
	COLLECT,
	PARTYMEMBERS,
	GOTO,
	IMPOSIBLE,
	UNKNOWN
};

class Quest
{
public:
	Quest() {};
	Quest(QuestType type) : type(type) {}

	virtual bool Update() { return false; }

public:

	SString name;
	SString description;
	QuestType type;
	int id;
	int nextQuestId;
	int reward;
	bool colleted = false;
};

#endif // __QUEST_H__