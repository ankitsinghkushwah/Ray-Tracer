
#ifndef INC_TIMER_H
#define INC_TIMER_H




//C++ headers
#include<memory>
#include<chrono>
#include<string>
#include<sstream>
#include<ctime>
#include<iomanip>


class timer
{

public:
		timer();
		void init();
		inline int get_frame_count() { return mCounter; }
		inline float get_elapsed_time() const {return mElapsedTime;}
		inline unsigned int get_FPS() const {return mFPS;}
		void reset();
		void start();
		void update();
		std::string get_current_time_and_date();

private:
		typedef std::chrono::high_resolution_clock mClock;
		typedef std::chrono::microseconds mMicroseconds;
		//for elapsed time
		mClock::time_point mLast;
		mClock::time_point mCurrent;
		float mElapsedTime;
		int mFrameCounter;
		//For FPS
		int mFPS;
		int mCounter;
		mClock::time_point mLastTime;
		float in_seconds(mClock::duration pDuration) const;
};


#endif