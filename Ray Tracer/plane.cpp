#include "plane.h"

plane::plane(const vec4& pNormal,
	const vec4& pPosition,
	const vec4& pColor,
	Material pMatType,
	ShapeID pID,
	float pRefFact)
	: 
	mNormal(pNormal),
	mColor(pColor),
	object(pPosition,pID,pMatType,pRefFact)
{
}

plane::~plane()
{
}

bool plane::hit(const ray & r, float tMin, float tMax, hit_record & hitRec)
{
	float denom = dot(r.mDirection, mNormal);
	vec4 p0l0 = mPosition - r.mOrigin;
	float t = p0l0.dot(mNormal) / denom;
	hitRec.normal = mNormal;
	hitRec.t = t;
	hitRec.hitPoint = r.point_at_parameter(t);
	return t > 0.01;
}

bool plane::hit_or_miss(const ray & r, float tMin, float tMax)
{
	float denom = dot(r.mDirection, mNormal);
		vec4 p0l0 = mPosition - r.mOrigin;
		float t = p0l0.dot(mNormal) / denom;
		return t > 0.01;
}
