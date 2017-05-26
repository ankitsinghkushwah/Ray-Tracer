#ifndef INC_HIT_RECORD_H
#define INC_HIT_RECORD_H


#include"vec3.h"


struct hit_record {
	vec3 normal;
	float t;
	vec3 hitPoint;
};

#endif