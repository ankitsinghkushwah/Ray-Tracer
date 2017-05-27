#ifndef INC_MATH_H
#define INC_MATH_H

#include"vec4.h"

//compile-time constants
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

inline int index(int row, int col){	return row * 4 + col; }
vec4 reflect(vec4 normal, vec4 lightDir);

/*famous Inverse Square root function from DOOM 3*/
float Q_rsqrt(float number);


#endif
