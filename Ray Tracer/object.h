#ifndef INC_OBJECT_H
#define INC_OBJECT_H

#include"ray.h"
#include"vec3.h"

class object {
public:
	virtual bool hit(const ray& r,float& hitPoint,vec3& normal,float tMin,float tMax) = 0;
	inline virtual vec3 get_color() = 0;
};

#endif