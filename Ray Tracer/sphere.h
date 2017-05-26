#ifndef INCLUDE_SPHERE_H
#define INCLUDE_SPHERE_H

#include"vec3.h"
#include"ray.h"
#include"object.h"
#include"enum_classes.h"

class sphere : public object{

private:

  vec3 mCenter;
  float mRadius;
  vec3 mColor;

public:

  
  sphere(const vec3& pCenter,
	  float pRadius,
	  const vec3& pColor,
	  Material pMatType,
	  ShapeID pID);

  virtual bool hit(const ray& r, float tMin,float tMax,hit_record& hitRec)override;
  virtual bool hit_or_miss(const ray& r,float tMin,float tMax) override;

  virtual vec3 get_color() override{ return mColor; }
  virtual Material get_material_type() override { return mMatType; }
  virtual ShapeID get_id() override { return mID; }


};

#endif