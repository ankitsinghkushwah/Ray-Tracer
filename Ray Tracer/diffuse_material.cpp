#include"diffuse_material.h"
#include"random.h"

diffuse_material::diffuse_material(const vec3 & a)
  : albedo(a)
{
}

bool diffuse_material::scatter(const ray & r,
  const hit_record & rec, 
  vec3 & attenuation, 
  ray & scattered) const
{
  vec3 target = rec.p + rec.normal + random::getInstance()->inside_unit_sphere();
  scattered = ray(rec.p,target-rec.p);
  attenuation = albedo;
  return true;
}
