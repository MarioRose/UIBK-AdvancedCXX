#ifndef TIMER_H
#define TIMER_H


#include <SDL.h>

//The application time based timer
class Timer {
public:
	//Initializes variables
	Timer();

	//The various clock actions
	void start();

	//Gets the timer's time
	Uint32 getTicks();


private:
	//The clock time when the timer started
	Uint32 mStartTicks;

	//The ticks stored when the timer was paused
	Uint32 mPausedTicks;

	//The timer status
	bool mPaused;
	bool mStarted;
};

#endif
