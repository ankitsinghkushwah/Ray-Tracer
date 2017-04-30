#include "vec3.h"


vec3::vec3(float x, float y, float z) {
  comps[0] = x;
  comps[1] = y;
  comps[2] = z;
}

vec3::vec3(const vec3 & other) {
  comps[0] = other[0];
  comps[1] = other[1];
  comps[2] = other[2];
}

vec3 & vec3::operator=( const vec3 & other) {
  if (this == &other) return *this;
  comps[0] = other.x();
  comps[1] = other.y();
  comps[2] = other.z();
  return *this;
}

vec3::~vec3() {
  
}

std::ostream & operator <<(std::ostream& cout, const vec3& other) {
  cout << "(" << other.comps[0] << "," << other.comps[1] << "," << other.comps[2] << ")\n";
  return cout;
}
