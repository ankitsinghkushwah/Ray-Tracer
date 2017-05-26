#include "plane.h"

plane::plane(const vec3& pNormal,
	const vec3& pPosition,
	const vec3& pColor,
	Material pMatType,
	ShapeID pID)
	: 
	mNormal(pNormal),
	mPosition(pPosition),
	mColor(pColor),
	object(pID,pMatType)
{
}

plane::~plane()
{
}

bool plane::hit(const ray & r, float tMin, float tMax, hit_record & hitRec)
{
	float denom = dot(r.mDirection, mNormal);

		vec3 p0l0 = mPosition - r.mOrigin;
		float t = p0l0.dot(mNormal) / denom;
		hitRec.normal = mNormal;
		hitRec.t = t;
		hitRec.hitPoint = r.point_at_parameter(t);
		return t > 0.01;
	
	return false;
}

bool plane::hit_or_miss(const ray & r, float tMin, float tMax)
{
	float denom = dot(r.mDirection, mNormal);
	//if (denom < 0.0) {
		vec3 p0l0 = mPosition - r.mOrigin;
		float t = p0l0.dot(mNormal) / denom;
		return t > 0.01;
	//}
	return false;
}
