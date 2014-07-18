#ifndef GROUP_H
#define GROUP_H

#include <list>
#include "Node.h"

class Group : public Node {
public:
	void addChild(Node * childNode);
	void removeChild();
	void removeChild(std::list<Node *>::iterator position);
	void removeChild(std::list<Node *>::iterator first, std::list<Node *>::iterator last);
	std::list<Node *> getChildNodes();

protected:
	std::list<Node *> childNodes;
};

#endif