#include "Timer.h"
#include <sstream>
#include <log.hpp>
#include <windows.h>

using RR::Log;

Timer::Timer()
: init(0), dt(0)
{
	reset();
}

double Timer::getTime()
{
	Log &log = Log::instance();
	__int64 currentTime;
	if(!QueryPerformanceCounter((LARGE_INTEGER*)(&currentTime)))
		log << "Timer could not get high resolution timer count for current time";
	return (double)(currentTime - init) / dt;
}

void Timer::reset()
{
	Log &log = Log::instance();
	if(!QueryPerformanceFrequency((LARGE_INTEGER*)&dt))
		log << "Timer could not get high resolution timer frequency";
	if(!QueryPerformanceCounter((LARGE_INTEGER*)(&init)))
		log << "Timer could not get high resolution timer count";
}