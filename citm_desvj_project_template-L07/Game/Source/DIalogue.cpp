#include "Dialogue.h"

Option::Option(pugi::xml_node option) {
	this->id = option.attribute("id").as_int();
	this->text = option.attribute("text").as_string();
	this->nextNodeId = option.attribute("nextNodeId").as_int();
	this->questCompletionId = option.attribute("questCompletionId").as_int();
	this->questTriggerId = option.attribute("questTriggerId").as_int();
}

Node::Node(pugi::xml_node node) {
	this->id = node.attribute("id").as_int();
	this->text = node.attribute("text").as_string();

	for (pugi::xml_node n = node.child("option"); n; n = n.next_sibling("option"))
	{
		Option* option = new Option(n);
		this->options.Add(option);
	}

	this->currentOption = SetCurrentOption(0);
}

Option* Node::SetCurrentOption(int id) {

	for (ListItem<Option*>* op = options.start; op != nullptr; op = op->next)
	{
		if (op->data->id == id) {
			return op->data;
		}
	}

	return nullptr;
}

Dialogue::Dialogue(pugi::xml_node dialogue) {
	this->id = dialogue.attribute("id").as_int();

	for (pugi::xml_node n = dialogue.child("node"); n; n = n.next_sibling("node"))
	{
		Node* node = new Node(n);
		this->nodes.Add(node);
	}

	this->currentNode = SetCurrentNode(0);
}

Dialogue::~Dialogue() {

}

Node* Dialogue::SetCurrentNode(int id) {
	for (ListItem<Node*>* n = nodes.start; n != nullptr; n = n->next)
	{
		if (n->data->id == id) {
			return n->data;
		}
	}

	return nullptr;
}