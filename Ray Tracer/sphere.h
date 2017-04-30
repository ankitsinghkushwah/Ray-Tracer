#ifndef INCLUDE_SPHERE_H
#define INCLUDE_SPHERE_H

#include"hitable.h"
#include"vec3.h"
#include"ray.h"
#include"material.h"

class sphere : public hitable {

private:

  material *mMaterial;
  vec3 mCenter;
  float mRadius;

public:
  
  sphere(const vec3& pCenter,float pRadius,material* pMaterial);
  
  virtual bool hit(const ray& ray,
    float tMin,
    float tMax,
    hit_record& hitRec) const override;

  inline vec3 getCenter() { return mCenter; }
  inline float getRadius() { return mRadius; }
  inline void setCenter(const vec3& pCenter) { mCenter = pCenter; }
  inline void setRadius(float pRadius) { mRadius = pRadius; }
  

};

#endif