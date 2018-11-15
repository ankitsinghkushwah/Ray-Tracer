#ifndef INC_HIT_RECORD_H
#define INC_HIT_RECORD_H


#include"vec4.h"


struct alignas(16) hit_record {
	vec4 normal;
	vec4 hitPoint;
	float t;
};

#endif