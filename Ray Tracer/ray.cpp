#include "ray.h"



ray::ray()
  :
  mOrigin(0.0f),
  mDirection(0.0f)
{

}

ray::ray(const vec3 & pOrigin, const vec3 & pDirection)
  :
  mOrigin(pOrigin),
  mDirection(pDirection)
{
}


ray::~ray()
{
}
