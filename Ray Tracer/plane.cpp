#include "plane.h"

plane::plane(const vec3& pNormal, const vec3& pPosition, const vec3& pColor)
	: 
	mNormal(pNormal),
	mPosition(pPosition),
	mColor(pColor)
{
}

plane::~plane()
{
}

/*bool plane::hit(const ray & r,float& hitPoint,float tMin, float tMax) 
{
	float denom = dot(r.get_direction(),mNormal);
	vec3 p0l0 = mPosition - r.get_origin();
	float t = p0l0.dot(mNormal) / denom;
	if (t >= 0) {
		
	}
	hitPoint = t;
	return t >= 0;
}*/
