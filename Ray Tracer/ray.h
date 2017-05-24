#ifndef INC_RAY_H
#define INC_RAY_H

#include"vec3.h"

class ray
{
public:

  //construction and destruction
  ray();
  ray(const vec3& pOrigin,const vec3& pDirection);
  ~ray();

  inline vec3 point_at_parameter(float t) const {
	  return ( mOrigin + ( t*mDirection ) ); 
  }

  vec3 mOrigin;
  vec3 mDirection;
  
};


#endif