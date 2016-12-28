#pragma once
#ifndef TIMER_H
#define TIMER_H

#if defined(_WIN32) || defined(_WIN64)   // Windows system specific
#include <windows.h>
#else          // Unix based system specific
#include <sys/time.h>
#endif

class Timer {
public:
	Timer();
	~Timer() {};

	void start();
	void stop();
	// get elapsed time in seconds
	double getElapsedTime();   
	double getElapsedTimeMicroSec();
private:
	double startTimeInMicroSec;                 
	double endTimeInMicroSec;                   
	bool    stopped;                             
#if defined(_WIN32) || defined(_WIN64)
	LARGE_INTEGER frequency;                    // ticks per second
	LARGE_INTEGER startCount;                   
	LARGE_INTEGER endCount;                     
#else
	timeval startCount;                         
	timeval endCount;                           
#endif
};

#endif // !TIMER_H

