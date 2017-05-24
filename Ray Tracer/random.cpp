#include"random.h"


using namespace std;

	random::random()
	{
		mRandEngine.seed(chrono::steady_clock::now().time_since_epoch().count());
	}

	random::~random()
	{
		
	}


	void random::update()
	{
		mRandEngine.seed(chrono::steady_clock::now().time_since_epoch().count());
	}

	
	float random::real(float pMin, float pMax)
	{
		std::uniform_real_distribution<> distReal(pMin,pMax);
		return distReal(mRandEngine);
	}


	int random::integer(int pMin, int pMax)
	{
		std::uniform_int_distribution<> distReal(pMin, pMax);
		return distReal(mRandEngine);
	}


  vec3 random::inside_unit_sphere() {
    vec3 p;
    do {
      p = vec3(real(-1.0f, 1.0f), real(-1.0f, 1.0f), real(-1.0f, 1.0f));
    } while (p.squared_length() >= 1.0);

    return p;
  }