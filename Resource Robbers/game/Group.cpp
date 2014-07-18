#include "Group.h"

void Group::addChild(Node * childNode) {
	childNodes.push_back(childNode);
}

void Group::removeChild() {
	childNodes.pop_back();
}

void Group::removeChild(std::list<Node *>::iterator position) {
	childNodes.erase(position);
}

void Group::removeChild(std::list<Node *>::iterator first, std::list<Node *>::iterator last) {
	childNodes.erase(first, last);
}

std::list<Node *> Group::getChildNodes() {
	return childNodes;
}