#include <string>
#include <iostream>
#include <sstream>
#include "Log.hpp"

using namespace RR;

Log *Log::_singleton = NULL;

void default_handler(const std::string &log, Log::Level l)
{
	switch (l)
	{
	case Log::RR_DEBUG:
	case Log::RR_INFO:
	case Log::RR_WARNING:
	case Log::RR_ERROR:
		std::clog<<log<<std::endl;
		break;
	case Log::RR_FATAL:
		std::clog<<log<<std::endl;
		exit(42);
	default:
		std::clog<<log<<std::endl;
	}
}

Log&
Log::instance()
{
	return ((_singleton) ? *_singleton : *(_singleton = new Log()));
}

void
Log::destroy()
{
	_singleton = NULL;
	delete (this);
}

Log::Log()
 : _handler(&::default_handler), _level(RR_DEBUG)
{
}

Log::Log(const Log &)
{

}

Log::~Log()
{
	std::clog<<std::endl<<"Flushing log"<<std::endl;
	flush();
	_singleton = NULL;
}

Log &
Log::operator=(const Log &)
{
	return (*this);
}

void (*
Log::handler(void (*new_handler)(const std::string &, Log::Level)))(const std::string &, Log::Level)
{
	void (*old_handler)(const std::string &, Level);
	old_handler = _handler;
	_handler = new_handler;
	return (old_handler);
}

void (*
Log::handler())(const std::string &, Log::Level)
{
	return (_handler);
}

Log &
Log::debug(const std::string &log)
{
	_handler(log, Log::RR_DEBUG);
	return (*this);
}

Log &
Log::info(const std::string &log)
{
	_handler(log, Log::RR_INFO);
	return (*this);
}

Log &
Log::warning(const std::string &log)
{
	_handler(log, Log::RR_WARNING);
	return (*this);
}

Log &
Log::error(const std::string &log)
{
	_handler(log, Log::RR_ERROR);
	return (*this);
}

Log &
Log::fatal(const std::string &log)
{
	*this<<"Fatal: "<< log;
	_handler(log, Log::RR_FATAL);
	return (*this);
}


void
Log::flush()
{
	std::ostringstream::flush();
	std::clog<<levelToStr(_level)<<": "<<this->str()<<std::endl;
	str(std::string());
}

std::ostringstream&
Log::level(Level l)
{
	_level = l;
	return (*(std::ostringstream *)this);
}

const char *
Log::levelToStr(Level l)
{
	static const char *level_def[] =
	{
		"Debug",
		"Info",
		"Warning",
		"Error",
		"Fatal"
	};
	
	return (level_def[l]);
}

/*
Log&
Log::operator<<(const std::ostream &os)
{
	std::clog<<levelToStr(_level)<<": "<<os<<std::endl;
	return (*this);
}
*/

Log&
Log::operator<<(Log::Level l)
{
	flush();
	_level = l;
	return (*this);
}