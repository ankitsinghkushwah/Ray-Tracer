#include "vec4.h"

#ifndef USE_SSE

//defining statics
vec4 vec4::ZERO = vec4(0.0f);
vec4 vec4::UP = vec4(0.0f,1.0f,0.0f);
vec4 vec4::DOWN = vec4(0.0f, -1.0f, 0.0f);
vec4 vec4::LEFT = vec4(-1.0f, 0.0f, 0.0f);
vec4 vec4::RIGHT = vec4(1.0f, 0.0f, 0.0f);
vec4 vec4::FORWARD = vec4(0.0f,0.0f,-1.0f);
vec4 vec4::BACKWARD = vec4(0.0f,0.0f,1.0f);


vec4::vec4():
	x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{}

vec4::vec4(float _x, float _y, float _z,float _w) :
	x(_x), y(_y), z(_z),w(_w)
{
}

vec4::vec4(float n) :
	x(n), y(n), z(n), w(1.0f)
{
}



vec4::vec4(const vec4 & other):
x(other.x), 
y(other.y), 
z(other.z),
w(1.0f)
{

}

vec4 & vec4::operator=( const vec4 & other) {
  if (this == &other) return *this;
  x = other.x;
  y = other.y;
  z = other.z;
  w = 1.0f;
  return *this;
}

vec4::~vec4() {
  
}


#else

//defining statics
vec4 vec4::ZERO = vec4(0.0f);
vec4 vec4::UP = vec4(0.0f, 1.0f, 0.0f);
vec4 vec4::DOWN = vec4(0.0f, -1.0f, 0.0f);
vec4 vec4::LEFT = vec4(-1.0f, 0.0f, 0.0f);
vec4 vec4::RIGHT = vec4(1.0f, 0.0f, 0.0f);
vec4 vec4::FORWARD = vec4(0.0f, 0.0f, -1.0f);
vec4 vec4::BACKWARD = vec4(0.0f, 0.0f, 1.0f);



vec4::vec4()
{
	_mm_set_ps(0.0f, 0.0f, 0.0f, 0.0f);
}

vec4::vec4(const vec4& other)
{
	vec = other.vec;
}

vec4::vec4(float _x, float _y, float _z, float _w)
{
	vec = _mm_set_ps(_w, _z, _y, _x);
}

vec4::vec4(float n)
{
	vec = _mm_set_ps(n, n, n, n);
}


vec4 & vec4::operator=(const vec4 & other) {
	if (this == &other) return *this;
	vec = other.vec;
	return *this;
}

vec4::~vec4() {

}

#endif
