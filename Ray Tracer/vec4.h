#ifndef INC_VEC4_H
#define INC_VEC4_H

#include<iostream>
#include<cassert>


class vec4
{
public:

	union {
		struct {	float x, y, z,w; };
		struct {    float r, g, b,a; };
	};

	//normalised directions
	static vec4 UP;
	static vec4 DOWN;
	static vec4 ZERO;
	static vec4 LEFT;
	static vec4 RIGHT;
	static vec4 FORWARD;
	static vec4 BACKWARD;

  //construction
	vec4();
  explicit vec4(float x , float y, float z ,float w = 1.0f) ;
  explicit vec4(float n);
  vec4(const vec4& other);
  vec4& operator = (const vec4& other);
  //destruction
  ~vec4();
 

  //vec4-vec4 arithmetic operations
  inline vec4 operator + (const vec4& other) const{
    return vec4( x+other.x,
      y + other.y,
      z + other.z);
  }
  inline vec4 operator - (const vec4& other) const {
    return vec4(x - other.x,
      y - other.y,
      z - other.z);
  }

  //vec4-vec4 arithmetic operations
  inline void operator += (const vec4& other) {
	  x += other.x;
	  y += other.y;
	  z += other.z;
  }


  inline vec4 operator * (const vec4& other) const {
    return vec4(x*other.x,
      y*other.y,
      z*other.z);
  }
  inline vec4 operator / (const vec4& other) const {
    assert((int)other.x!=0 && (int)other.y!=0 && (int)other.z!=0);
    return vec4(x / other.x,
      y / other.y,
      z / other.z);
  }
  inline void operator *= (const vec4& other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
  }
  inline void operator /= (const vec4& other) {
     x /= other.x;
     y /= other.y;
     z /= other.z;
  }
  //vec4-scalar * & /
 
  inline vec4 operator / (float scalar) const {
    assert(scalar>0.00000f);
    return vec4(x / scalar, y / scalar, z/scalar);
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

  inline vec4 normalize() const {
    vec4 v;
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

  inline vec4& make_itzero() {
    x = y = z = 0;
    return *this;
  }
  
  inline float dot(const vec4& other) const {
    return ((x*other.x)+(y*other.y)+(z*other.z));
  }

  inline vec4 cross(const vec4& other) const {
    return vec4(
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
  friend std::ostream& operator << (std::ostream& cout, const vec4& other);
};

//non-member inline operators
inline vec4 operator * (const vec4& v,float scalar) {
  return vec4(v.x * scalar, v.y * scalar, v.z * scalar);
}

inline vec4 operator * (float scalar, const vec4& v) {
  return vec4(v.x * scalar, v.y * scalar, v.z * scalar);
}

inline float dot(const vec4& v1, const vec4& v2) {
  return (v1.x*v2.x + v1.y*v2.y+v1.z*v2.z);
}


#endif