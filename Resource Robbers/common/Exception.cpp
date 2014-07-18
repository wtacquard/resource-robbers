#include <exception>
#include <iostream>
#include <string>
#include "Exception.hpp"


using namespace RR;

Exception::Exception(const char *str)
: std::exception(str)
{

}

Exception::~Exception()
{

}

const char *Exception::what() const throw()
{
	static std::string	err;

	err = "Ressource Robbers: ";
	return (err.append(std::exception::what()).c_str());
}