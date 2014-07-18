#ifndef COMMANDS_H_
# define COMMANDS_H_

#include <string>
#include <boost/ptr_container/ptr_set.hpp>
#include "Command.h"

class Commands
{
	boost::ptr_set<Command> commands;
private:
	void resetToDefault();

public:
	Commands();
	~Commands();
	Command* getCommand(const std::string& name);
	bool load(std::string fileName);
	void parseXml(const std::string& xml, std::string& name, bool& isDown, bool& isAltDown);
	void setupForClient(RR::Socket&);
	void setupForServer(Player* player);
	boost::ptr_set<Command>& getCommands();
};

#endif //!COMMANDS_H_