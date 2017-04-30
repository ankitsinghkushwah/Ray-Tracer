#ifndef INC_MATERIAL_H
#define INC_MATERIAL_H

#include"ray.h"
#include"hitable.h"
#include"vec3.h"

class material {
public:
  virtual bool scatter(const ray& r,const hit_record& rec,vec3& attenuation,ray& scattered) const = 0;
};


#endif
