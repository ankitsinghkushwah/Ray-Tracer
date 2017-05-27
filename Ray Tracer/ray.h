#ifndef INC_RAY_H
#define INC_RAY_H

#include"vec4.h"

class ray
{
public:

  //construction and destruction
  ray();
  ray(const vec4& pOrigin,const vec4& pDirection);
  ~ray();

  inline vec4 point_at_parameter(float t) const {
	  return ( mOrigin + ( t*mDirection ) ); 
  }

  vec4 mOrigin;
  vec4 mDirection;
  
};


#endif