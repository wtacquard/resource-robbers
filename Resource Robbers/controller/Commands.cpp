#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>
#include <tinyxml.h>
#include <string>

#include <exception>
#include "Exception.hpp"
#include "SocketException.hpp"
#include "log.hpp"

#include <winsock2.h>
#include "Socket.hpp"
#include "Commands.h"
#include "SocketClient.hpp"

#include "DXUT.h"
#include "server/Object.h"
#include "game/WorldObject.h"
#include "game/DisplayObject.h"
#include "common/ClientController.h"

using std::string;
using boost::ptr_set;
using RR::Socket;
using RR::Log;

Commands::Commands()
{
	resetToDefault();
}

Commands::~Commands()
{
	commands.clear();
}

ptr_set<Command>& Commands::getCommands()
{
	return commands;
}

Command* Commands::getCommand(const string& name)
{
	Command c(name, 0, NULL);
    return &*commands.find(c);
}

bool Commands::load(string name)
{
	resetToDefault();
	TiXmlDocument doc(name.c_str());

	doc.LoadFile();
	/*
	if(!doc.LoadFile())
	{
		log << "Failed to open commands file: " << name;
		return false;
	}

	TiXmlHandle docHandle(&doc);
	TiXmlElement* element;
	TiXmlHandle root(0);
	element = doc.FirstChildElement().Element();

	if (!element)
	{
		log << "Improper XML in commands file: No root found";
		return false;
	}
	root = TiXmlHandle(element);
	element = root.FirstChild("command").Element();
	while(element && element = element->NextSiblingElement())
	{
		TiXmlElement* name;
		TiXmlElement* key;
		if(!(name = element.FirstChild("name").Element()))
			continue;
		if(!(key = element.FirstChild("key").Element()))
			continue;
		string *pKey=pElem->Value();
		string *pText=pElem->GetText();
		if (pKey && pText) 
		{
			m_messages[pKey]=pText;
		}
	}
	*/
	return false;
}

void Commands::setupForClient(Socket& socket)
{
	getCommand("moveForward")->bindToNetworkSend(socket);
	getCommand("moveBackward")->bindToNetworkSend(socket);
	getCommand("moveLeft")->bindToNetworkSend(socket);
	getCommand("moveRight")->bindToNetworkSend(socket);
	getCommand("fire")->bindToNetworkSend(socket);
}
void Commands::setupForServer(Player* player)
{
	getCommand("moveForward")->bindToSetDirection(player);
	getCommand("moveBackward")->bindToSetDirection(player);
	getCommand("moveLeft")->bindToSetDirection(player);
	getCommand("moveRight")->bindToSetDirection(player);
	//getCommand("fire")->bindToSetKeyState(player);
}
void Commands::resetToDefault()
{
/*	Command *test = new Command("iworeiw", 'X', NULL);
	commands.insert(test);
	commands.clear();
	delete test;*/
	commands.insert(new Command("moveForward", 'W', NULL));
	commands.insert(new Command("moveBackward", 'S', NULL));
	commands.insert(new Command("moveLeft", 'A', NULL));
	commands.insert(new Command("moveRight", 'D', NULL));
	commands.insert(new Command("fire", VK_LBUTTON, NULL));
}

