#ifndef INCLUDE_SPHERE_H
#define INCLUDE_SPHERE_H

#include"vec3.h"
#include"ray.h"
#include"object.h"
#include"global_settings.h"

class sphere {

private:

  vec3 mCenter;
  float mRadius;
  vec3 mColor;
  

public:

	global_settings::MATERIAL mat;
  sphere(const vec3& pCenter,float pRadius,const vec3& pColor,global_settings::MATERIAL material);
  inline vec3 getCenter() { return mCenter; }
  inline float getRadius() { return mRadius; }
  inline void setCenter(const vec3& pCenter) { mCenter = pCenter; }
  inline void setRadius(float pRadius) { mRadius = pRadius; }
  
  bool hit(const ray& r, double& hitPoint,vec3& normal,double tMin,double tMax);
  bool hitOrNot(const ray& r,float tMin,float tMax);

  vec3 get_color(){ return mColor; }


};

#endif