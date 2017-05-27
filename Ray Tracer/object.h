#ifndef INC_OBJECT_H
#define INC_OBJECT_H

#include"ray.h"
#include"vec4.h"
#include"enum_classes.h"
#include"hit_record.h"


class object {

protected:
	
	ShapeID mID;
	Material mMatType;
	float mRefFact;
public:

	vec4 mPosition;

	object(vec4 mPosition,ShapeID pID,Material mMatType,float pRefFact);
	virtual bool hit(const ray& r,float tMin, float tMax,hit_record& hitRec) = 0;
	virtual bool hit_or_miss(const ray& r,float tMin, float tMax) = 0;
	virtual vec4 get_color() = 0;
	virtual ShapeID get_id() = 0;
	virtual Material get_material_type() = 0;
	virtual float get_ref_fact() = 0;
};

#endif