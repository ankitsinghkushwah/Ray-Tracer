//Declaration header
#include"timer.h"


//Exposing namespace
using namespace std::chrono;


	//Private functions


	float timer::in_seconds(mClock::duration pDuration) const
	{
		return (duration_cast<microseconds>(pDuration).count() / 1000000.0);
	}


	//Public functions
	


	timer::timer()
	{
		mCounter = 0;
		mFPS = 0;
		mFrameCounter = 0;
	}


	void timer::init()
	{
		mCounter = 0;
		mFPS = 0;
	}


	void timer::start() {
		mLast = mClock::now();
		mLastTime = mClock::now();
	}


	void timer::reset() {
		
		mLast = mClock::now();
		mLastTime = mClock::now();

		mCounter = 0;
		mFPS = 0;
	}


	void timer::update()
	{

		//Updating mElapsedTime
		mCurrent = mClock::now();
		mElapsedTime = in_seconds(mCurrent-mLast);
		mLast = mCurrent;

		//Updating fps

		mCounter++;

		mFrameCounter++;
		if (in_seconds(mCurrent - mLastTime) > 1.0f)
		{
			mFPS = mFrameCounter;
			mFrameCounter = 0;
			mLastTime = mCurrent;
		}

	}

	std::string timer::get_current_time_and_date() {
			auto now = std::chrono::system_clock::now();
			auto in_time_t = std::chrono::system_clock::to_time_t(now);

			std::stringstream ss;
			ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
			return ss.str();
	
	}

