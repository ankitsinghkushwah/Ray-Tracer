#ifndef INC_RANDOM_H
#define INC_RANDOM_H


//C++ headers
#include<random>
#include<chrono>
#include<memory>

#include"vec3.h"



	class random
	{


	public:

		~random();

		static std::shared_ptr<random> getInstance()
		{
			static std::shared_ptr<random> instance(new random);
			return instance;
		}

		void update();

		float real(float pMin, float pMax);

		int integer(int pMin, int pMax);

    vec3 inside_unit_sphere();


	private:

		std::default_random_engine mRandEngine;

		random();


	};


#endif