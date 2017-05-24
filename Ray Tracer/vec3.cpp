#include "vec3.h"


//defining statics
vec3 vec3::ZERO = vec3(0.0f);
vec3 vec3::UP = vec3(0.0f,1.0f,0.0f);
vec3 vec3::DOWN = vec3(0.0f, -1.0f, 0.0f);
vec3 vec3::LEFT = vec3(-1.0f, 0.0f, 0.0f);
vec3 vec3::RIGHT = vec3(1.0f, 0.0f, 0.0f);
vec3 vec3::FORWARD = vec3(0.0f,0.0f,-1.0f);
vec3 vec3::BACKWARD = vec3(0.0f,0.0f,1.0f);


vec3::vec3(float _x, float _y, float _z) :
	x(_x), y(_y), z(_z) {}

vec3::vec3(const vec3 & other):
x(other.x), 
y(other.y), 
z(other.z)
{

}

vec3 & vec3::operator=( const vec3 & other) {
  if (this == &other) return *this;
  x = other.x;
  y = other.y;
  z = other.z;
  return *this;
}

vec3::~vec3() {
  
}

std::ostream & operator <<(std::ostream& cout, const vec3& other) {
  cout << "(" << other.x << "," << other.y << "," << other.z << ")\n";
  return cout;
}
