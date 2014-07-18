#ifndef TIMER_H
#define TIMER_H

class Timer
{
	__int64 init;
	__int64 dt;
public:
	Timer();
	double getTime();
	void reset();
};

#endif TIMER_H