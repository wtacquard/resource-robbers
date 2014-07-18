#ifndef LOG_HPP_
# define LOG_HPP_

#include <iostream>
#include <sstream>

namespace RR
{
	class Log : public std::ostringstream
	{
	public:
		enum Level
		{
			/*prefix with RR_ because there is a conflict with some of them 
			that exist as MACRO under Windows*/
			RR_DEBUG = 0,
			RR_INFO,
			RR_WARNING,
			RR_ERROR,
			RR_FATAL
		};

	private:
		static  Log		*_singleton;
		Level			_level;

		void (*_handler)(const std::string &, Log::Level);
		static const char *levelToStr(Level);
		
		Log();
		Log(const Log &);
		Log &operator=(const Log &);

	public:
		static Log &instance();
		void   destroy();
		~Log();

	

		void (*handler(void (*new_handler)(const std::string &, Log::Level)))(const std::string &, Log::Level);
		void (*handler())(const std::string &, Level);
		void flush();
	
		Log &debug(const std::string &);
		Log &info(const std::string &);
		Log &warning(const std::string &);
		Log &error(const std::string &);
		Log &fatal(const std::string &);

		std::ostringstream &level(Level);
		
		virtual Log& operator<<(Log::Level l);
		//virtual Log& operator<<(const std::ostream &os);

		template <class T> std::ostream& operator<<(const T &t);

		//friend std::ostream&	operator<<(const std::ostream &os, RR::Log::Level l);
	};
	void default_handler(const std::string &log, Log::Level);
	template <class T>
	std::ostream&
		Log::operator<<(const T &t)
	{
		std::clog<<levelToStr(_level)<<": "<<t;
		return (std::clog);
	}
}




#endif //!LOG_HPP_