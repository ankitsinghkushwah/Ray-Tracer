#ifndef INC_MATH_H
#define INC_MATH_H

#include"vec3.h"

const float PIE = 3.1415926535;
const float ONE_RADIAN_IN_DEGREES = 180.0f / PIE;

inline float to_radians(float angleInDeg) {
	return (angleInDeg/ONE_RADIAN_IN_DEGREES);
}

inline float to_degrees(float angleInRad) {
	return (angleInRad*(ONE_RADIAN_IN_DEGREES));
}

inline float max(float first, float second) {
	return (first > second ? first : second);
}

vec3 reflect(vec3 normal, vec3 lightDir);

vec3 refract(vec3 normal, vec3 lightDir);

float Q_rsqrt(float number);


#endif
