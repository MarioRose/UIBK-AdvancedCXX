#include <Timer.h>

#include <SDL.h>
#include <stdio.h>
#include <string>

Timer::Timer() {
	//Initialize the variables
	mStartTicks = 0;
	mPausedTicks = 0;

	mPaused = false;
	mStarted = false;
}

void Timer::start() {
	//Start the timer
	mStarted = true;

	//Unpause the timer
	mPaused = false;

	//Get the current clock time
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

Uint32 Timer::getTicks() {
	//The actual timer time
	Uint32 time = 0;

	//If the timer is running
	if( mStarted ) {
		//If the timer is paused
		if( mPaused ) {
			//Return the number of ticks when the timer was paused
			time = mPausedTicks;
		}
		else {
			//Return the current time minus the start time
			time = SDL_GetTicks() - mStartTicks;
		}
	}

	return time;
}
