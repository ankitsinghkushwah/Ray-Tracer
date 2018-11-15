#ifndef INC_RAY_H
#define INC_RAY_H

#include"vec4.h"

_declspec(align(16))
struct ray
{
	vec4 mOrigin;
	vec4 mDirection;

public:

  //construction and destruction
  ray();
  ray(const vec4& pOrigin,const vec4& pDirection);
  ~ray();

  inline vec4 point_at_parameter(float t) const 
  {
	  vec4 resultDir;
	  mDirection.mul(t,resultDir);
	  vec4 addResult;
	  mOrigin.add(resultDir,addResult);
	  return addResult;
  }

  
};


#endif