#include "vec4.h"

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
