#include "camera.h"

camera::camera()
  :
  mOrigin(0.0f,0.0f,0.0f),
  mLowerLeftCorner(-6.0f,-6.0f,-6.0f),
  mVOffset(0.0f,12.0f,0.0f),
  mHOffset(12.0f,0.0f,0.0f)
{
}

ray camera::get_ray(float u, float v) {
  return ray(mOrigin,(mLowerLeftCorner+u*mHOffset+v*mVOffset) - mOrigin);
}
