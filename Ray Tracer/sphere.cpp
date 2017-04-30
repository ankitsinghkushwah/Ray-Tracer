#include"sphere.h"

sphere::sphere(const vec3 & pCenter, float pRadius,material* pMaterial)
  : 
  mCenter(pCenter),
  mRadius(pRadius),
  mMaterial(pMaterial)
{

}

bool sphere::hit(const ray & r, float tMin, float tMax, hit_record & hitRec) const
{
  vec3 oc = r.get_origin() - mCenter;
  float a = dot(r.get_direction(), r.get_direction());
  float b = 2.0f*dot(oc, r.get_direction());
  float c = dot(oc, oc) - mRadius*mRadius;
  float d = b*b - 4 * a*c;
  
  if (d > 0) {

    float t = (-b - sqrt(d)) / (2.0f*a);
    if (t < tMax && t > tMin)
    {
      hitRec.t = t;
      hitRec.p = r.point_at_parameter(t);
      hitRec.normal = (hitRec.p - mCenter) / mRadius;
      hitRec.mat = mMaterial;
      return true;
    }

    t = (-b + sqrt(d)) / (2.0f*a);
    
    if (t < tMax && t > tMin)
    {
      hitRec.t = t;
      hitRec.p = r.point_at_parameter(t);
      hitRec.normal = (hitRec.p - mCenter) / mRadius;
      hitRec.mat = mMaterial;
      return true;
    }

  }

  return false;
}
