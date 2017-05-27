#include "ray.h"



ray::ray()
  :
  mOrigin(0.0f),
  mDirection(0.0f)
{

}

ray::ray(const vec4 & pOrigin, const vec4 & pDirection)
  :
  mOrigin(pOrigin),
  mDirection(pDirection)
{
}

ray::~ray()
{
}
