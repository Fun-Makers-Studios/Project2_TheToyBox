#ifndef __DIALOGUE_H__
#define __DIALOGUE_H__

#include "SString.h"
#include "List.h"

#include "PugiXml/src/pugixml.hpp"

class Option
{
public:
	Option(pugi::xml_node option);
	~Option() {};

public:
	int id;
	int nextNodeId;
	int questCompletionId;
	int questTriggerId;
	SString text;
	SString speaker;
};

class Node
{
public:
	Node(pugi::xml_node node);
	~Node() {};

	Option* SetCurrentOption(int id);

public:
	int id;
	SString text;
	SString speaker;

	List<Option*> options;
	Option* currentOption;
};

class Dialogue
{
public:
	Dialogue(pugi::xml_node dialogue);
	~Dialogue();

	Node* SetCurrentNode(int id);

public:
	int id;

	Node* currentNode;
	List<Node*> nodes;
};

#endif // __DIALOGUE_H__