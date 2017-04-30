#ifndef INC_CAMERA_H
#define INC_CAMERA_H

#include"vec3.h"
#include"ray.h"

class camera
{

private:

  vec3 mOrigin;
  vec3 mLowerLeftCorner;
  vec3 mVOffset;
  vec3 mHOffset;

public :

  camera();
  ray get_ray(float u,float v);

};

#endif