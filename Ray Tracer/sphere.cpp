#include"sphere.h"
#include"math.h"

//#define USE_USUAL_SQRT

sphere::sphere(const vec3 & pCenter, 
	float pRadius, 
	const vec3& pColor, 
	Material pMatType,
	ShapeID pID)
	:
  mCenter(pCenter),
  mRadius(pRadius),
	mColor(pColor),
	object(pID,pMatType)
{

}

bool sphere::hit(const ray & r, float tMin,float tMax,hit_record& hitRec)
{
	vec3 oc = r.mOrigin - mCenter;
	float a = 1.0f;
	float b = 2.0f*dot(oc, r.mDirection);
	float c = dot(oc, oc) - mRadius*mRadius;
	float d = b*b - 4 * a*c;

	if (d > 0) {

#ifdef USE_USUAL_SQRT
		float sqrtD = Q_rsqrt(d);
#else 
		float sqrtD = sqrt(d);
#endif
		float t = (-b - sqrtD) / (2.0*a);
		
		/* calculating roots */

		if (t < tMax && t > tMin) {
			hitRec.t = t;
			hitRec.hitPoint = r.point_at_parameter(t);
			hitRec.normal = (hitRec.hitPoint - mCenter) / mRadius;
			return true;
		}
		//calculating second root if the first root isnt greater than 0
		t = (-b + sqrtD) / (2.0*a);
		if (t < tMax && t > tMin) {
			hitRec.t = t;
			hitRec.hitPoint = r.point_at_parameter(t);
			hitRec.normal = (hitRec.hitPoint - mCenter) / mRadius;
			return true;
		}

	}

	return false;
}

bool sphere::hit_or_miss(const ray & r,float tMin,float tMax)
{
	vec3 oc = r.mOrigin - mCenter;
	float a = 1.0f;
	float b = 2.0f*dot(oc, r.mDirection);
	float c = dot(oc, oc) - mRadius*mRadius;
	float d = b*b - 4 * a*c;
	if (d > 0) {

#ifdef USE_USUAL_SQRT
		float sqrtD = Q_rsqrt(d);
#else 
		float sqrtD = sqrt(d);
#endif

		float t = (-b - sqrtD) / (2.0*a);
		//calculating first root
		if (t < tMax && t > tMin) {
			return true;
		}
		//calculating second root if the first root isnt greater than 0
		t = (-b + sqrtD) / (2.0*a);
		if (t < tMax && t > tMin) {
			return true;
		}

	}

	return false;
}

