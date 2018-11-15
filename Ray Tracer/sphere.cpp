#include"sphere.h"
#include<cmath>

//#define USE_USUAL_SQRT

sphere::sphere()
{
}

sphere::sphere(const vec4 & pCenter, 
	float pRadius, 
	const vec4& pColor, 
	Material pMatType,
	ShapeID pID,
	float pRefFact)
	:
  mRadius(pRadius),
	mColor(pColor),
	object(pCenter,pID,pMatType,pRefFact)
{

}

bool sphere::hit(const ray & r, float tMin,float tMax,hit_record& hitRec)
{
	vec4 oc;
	r.mOrigin.sub(mPosition,oc);
	float a = 1.0f;
	float dot1Result;
	dot(oc, r.mDirection,dot1Result);
	float b = 2.0f*dot1Result;
	float dot2Result;
	dot(oc, oc, dot2Result);
	float c = dot2Result - mRadius*mRadius;
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
			vec4 subResult;
			hitRec.hitPoint.sub(mPosition,subResult);
			subResult.div(mRadius,hitRec.normal);
			return true;
		}
		//calculating second root if the first root isnt greater than 0
		t = (-b + sqrtD) / (2.0*a);
		if (t < tMax && t > tMin) {
			hitRec.t = t;
			hitRec.hitPoint = r.point_at_parameter(t);
			vec4 subResult;
			hitRec.hitPoint.sub(mPosition, subResult);
			subResult.div(mRadius, hitRec.normal);
			return true;
		}

	}

	return false;
}

bool sphere::hit_or_miss(const ray & r,float tMin,float tMax)
{
	vec4 oc;
	r.mOrigin.sub(mPosition, oc);
	float a = 1.0f;
	float dot1Result;
	dot(oc, r.mDirection, dot1Result);
	float b = 2.0f*dot1Result;
	float dot2Result;
	dot(oc, oc, dot2Result);
	float c = dot2Result - mRadius * mRadius;
	float d = b * b - 4 * a*c;

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

