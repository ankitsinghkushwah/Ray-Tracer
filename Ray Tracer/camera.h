#ifndef INC_CAMERA_H
#define INC_CAMERA_H

#include"vec4.h"

struct alignas(16) camera {
	vec4 mPosition;
	float mFOV;
};

#endif