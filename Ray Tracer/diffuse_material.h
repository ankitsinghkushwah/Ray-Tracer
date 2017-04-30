#ifndef INC_DIFFUSE_MATERIAL_H
#define INC_DIFFUSE_MATERIAL_H

#include"material.h"

class diffuse_material : public material 
{
public:

  diffuse_material(const vec3& a);
  virtual bool scatter(const ray& r, 
    const hit_record& rec, 
    vec3& attenuation, 
    ray& scattered)const override;

  vec3 albedo;
};


#endif