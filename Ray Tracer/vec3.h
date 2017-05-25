#ifndef INC_VEC3_H
#define INC_VEC3_H

#include<iostream>
#include<cassert>
//#include"math.h"

class vec3
{
public:

	union {

		struct {
			float x, y, z;
		};

		struct {
			float r, g, b;
		};
	};


	//normalized directions
	static vec3 UP;
	static vec3 DOWN;
	static vec3 ZERO;
	static vec3 LEFT;
	static vec3 RIGHT;
	static vec3 FORWARD;
	static vec3 BACKWARD;

  //construction
  vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f);
  vec3(const vec3& other);
  vec3& operator = (const vec3& other);
  //destruction
  ~vec3();
  

  //vec3-vec3 arithmetic operations
  inline vec3 operator + (const vec3& other) const{
    return vec3( x+other.x,
      y + other.y,
      z + other.z);
  }
  inline vec3 operator - (const vec3& other) const {
    return vec3(x - other.x,
      y - other.y,
      z - other.z);
  }

  //vec3-vec3 arithmetic operations
  inline void operator += (const vec3& other) {
	  x += other.x;
	  y += other.y;
	  z += other.z;
  }


  inline vec3 operator * (const vec3& other) const {
    return vec3(x*other.x,
      y*other.y,
      z*other.z);
  }
  inline vec3 operator / (const vec3& other) const {
    assert((int)other.x!=0 && (int)other.y!=0 && (int)other.z!=0);
    return vec3(x / other.x,
      y / other.y,
      z / other.z);
  }
  inline void operator *= (const vec3& other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
  }
  inline void operator /= (const vec3& other) {
     x /= other.x;
     y /= other.y;
     z /= other.z;
  }
  //vec3-scalar * & /
 
  inline vec3 operator / (float scalar) const {
    assert(scalar>0.00000f);
    return vec3(x / scalar, y / scalar, z/scalar);
  }
  inline void operator *= (float scalar) {
    x *= scalar; 
    y *= scalar; 
    z *= scalar;
  }
  inline void operator /= (float scalar) {
    assert(scalar>0.00000000f);
    x /= scalar; 
    y /= scalar; 
    z /= scalar;
  }

  inline vec3 normalize() const {
    vec3 v;
    v.x = x / length();
    v.y = y / length();
    v.z = z / length();
    return v;
  }


  inline float length() const{
    return (sqrt((x*x)+(y*y)+(z*z)));
  }


  inline float squared_length() const {
    return x * x + y * y + z * z;
  }

  inline void make_it_unit() {
    
    x /= length();
    y /= length();
    z /= length();

  }

  inline vec3& make_itzero() {
    x = y = z = 0;
    return *this;
  }
  inline float dot(const vec3& other) const {
    return ((x*other.x)+(y*other.y)+(z*other.z));
  }
  inline vec3 cross(const vec3& other) const {
    return vec3(
    y*other.z+z*other.y,
      z*other.x+x*other.z,
      x*other.y+y*other.x
    );
  }
  //checks 
  bool check_ifzero() const{
    //typecasting the floats to int since its hard to compare two floats against each other
    bool result = (((int)x) == 0 && ((int)y) == 0 && ((int)z) == 0);
    return result;
  }
  //IO
  friend std::ostream& operator << (std::ostream& cout, const vec3& other);

};


//non-member inline operators
inline vec3 operator * (const vec3& v,float scalar) {
  return vec3(v.x * scalar, v.y * scalar, v.z * scalar);
}

inline vec3 operator * (float scalar, const vec3& v) {
  return vec3(v.x * scalar, v.y * scalar, v.z * scalar);
}


inline float dot(const vec3& v1, const vec3& v2) {
  return (v1.x*v2.x + v1.y*v2.y+v1.z*v2.z);
}
#endif