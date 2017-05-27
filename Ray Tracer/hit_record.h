#ifndef INC_HIT_RECORD_H
#define INC_HIT_RECORD_H


#include"vec4.h"


struct hit_record {
	vec4 normal;
	float t;
	vec4 hitPoint;
};

#endif