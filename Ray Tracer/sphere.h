#ifndef INCLUDE_SPHERE_H
#define INCLUDE_SPHERE_H

#include"vec4.h"
#include"ray.h"
#include"object.h"
#include"enum_classes.h"

class sphere : public object{

private:

  float mRadius;
  vec4 mColor;

public:

  
  sphere(const vec4& pCenter,
	  float pRadius,
	  const vec4& pColor,
	  Material pMatType,
	  ShapeID pID,
	  float pRefFact);

  virtual bool hit(const ray& r, float tMin,float tMax,hit_record& hitRec)override;
  virtual bool hit_or_miss(const ray& r,float tMin,float tMax) override;

  virtual vec4 get_color() override{ return mColor; }
  virtual Material get_material_type() override { return mMatType; }
  virtual ShapeID get_id() override { return mID; }
  virtual float get_ref_fact() override { return mRefFact; }


};

#endif