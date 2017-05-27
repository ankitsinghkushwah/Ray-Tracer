#ifndef INC_PLANE_H
#define INC_PLANE_H

#include"vec4.h"
#include"ray.h"
#include"object.h"


class plane : public object{
	//defines the orientation
	vec4 mNormal;
	//defines the color of the plane
	vec4 mColor;
public:
	plane(const vec4& pNormal,
		const vec4& mPosition,
		const vec4& pColor,
		Material pMatType,
		ShapeID pID,
		float pRefFact);
	~plane();

	virtual bool hit(const ray& r, float tMin, float tMax, hit_record& hitRec)override;
	virtual bool hit_or_miss(const ray& r, float tMin, float tMax)override;
	virtual vec4 get_color()override { return mColor; }
	virtual Material get_material_type()override { return mMatType; }
	virtual ShapeID get_id() override { return mID; }
	virtual float get_ref_fact() override { return mRefFact; }
};

#endif