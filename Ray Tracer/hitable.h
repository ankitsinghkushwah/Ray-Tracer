#ifndef INCLUDE_HITABLE_H
#define INCLUDE_HITABLE_H

#include"vec3.h"
#include"ray.h"

class material;

struct hit_record
{
  float t;
  vec3 p;
  vec3 normal;
  material* mat;
};


class hitable
{
public:

  virtual bool hit(const ray& ray,
    float tMin,
    float tMax,
    hit_record& hitRec) const = 0;
};

#endif
