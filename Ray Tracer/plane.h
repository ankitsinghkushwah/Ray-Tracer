#ifndef INC_PLANE_H
#define INC_PLANE_H

#include"vec3.h"
#include"ray.h"
#include"object.h"


class plane {

	//defines the orientation
	vec3 mNormal;
	//defines the distance from world-origin
	vec3 mPosition;
	//defines the color of the plane
	vec3 mColor;

public:

	plane(const vec3& pNormal,const vec3& mPosition,const vec3& pColor);
	~plane();
	//virtual bool hit(const ray& r, float& hitPoint,float tMin,float tMax) override;
//	inline virtual vec3 get_color() override{ return mColor; }

};

#endif