#ifndef INC_PLANE_H
#define INC_PLANE_H

#include"vec3.h"
#include"ray.h"
#include"object.h"


class plane : public object{
	//defines the orientation
	vec3 mNormal;
	//defines the distance from world origin
	vec3 mPosition;
	//defines the color of the plane
	vec3 mColor;
	//defines how much it will reflect
public:
	plane(const vec3& pNormal,
		const vec3& mPosition,
		const vec3& pColor,
		Material pMatType,
		ShapeID pID);
	~plane();

	virtual bool hit(const ray& r, float tMin, float tMax, hit_record& hitRec)override;
	virtual bool hit_or_miss(const ray& r, float tMin, float tMax)override;
	virtual vec3 get_color()override { return mColor; }
	virtual Material get_material_type()override { return mMatType; }
	virtual ShapeID get_id() override { return mID; }
};

#endif