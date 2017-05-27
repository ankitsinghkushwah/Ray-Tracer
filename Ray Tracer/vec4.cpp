#include "vec4.h"


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



std::ostream & operator <<(std::ostream& cout, const vec4& other) {
  cout << "(" << other.x << "," << other.y << "," << other.z << ")\n";
  return cout;
}
