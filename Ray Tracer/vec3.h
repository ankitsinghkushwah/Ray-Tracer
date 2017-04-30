#ifndef INC_VEC3_H
#define INC_VEC3_H

#include<iostream>
#include<cassert>

class vec3
{
public:
  //construction
  vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f);
  vec3(const vec3& other);
  vec3& operator = (const vec3& other);
  //destruction
  ~vec3();
  //getters
  inline float x() const { return comps[0]; }
  inline float y() const { return comps[1]; }
  inline float z() const { return comps[2]; }
  inline float r() const { return comps[0]; }
  inline float g() const { return comps[1]; }
  inline float b() const { return comps[2]; }
  
  //for non-const objects
  inline float& operator[](int index) {
    return comps[index];
  }

  //for const objects
  inline float operator[](int index) const {
    return comps[index];
  }
 
  //vec3-vec3 arithmetic operations
  inline vec3 operator + (const vec3& other) const{
    return vec3( comps[0]+other[0],
      comps[1] + other[1],
      comps[2] + other[2]);
  }
  inline vec3 operator - (const vec3& other) const {
    return vec3(comps[0] - other[0],
      comps[1] - other[1],
      comps[2] - other[2]);
  }
  inline vec3 operator * (const vec3& other) const {
    return vec3(comps[0]*other[0],
      comps[1]*other[1],
      comps[2]*other[2]);
  }
  inline vec3 operator / (const vec3& other) const {
    assert((int)other[0]!=0 && (int)other[1]!=0 && (int)other[2]!=0);
    return vec3(comps[0] / other[0],
      comps[1] / other[1],
      comps[2] / other[2]);
  }
  inline void operator *= (const vec3& other) {
    comps[0] *= other[0];
    comps[1] *= other[1];
    comps[2] *= other[2];
  }
  inline void operator /= (const vec3& other) {
     assert((int)other[0]!=0 && (int)other[1]!=0 && (int)other[2]!=0);
     comps[0] /= other[0];
     comps[1] /= other[1];
     comps[2] /= other[2];
  }
  //vec3-scalar * & /
 
  inline vec3 operator / (float scalar) const {
    assert(scalar>0.00000000f);
    return vec3(comps[0] / scalar, comps[1] / scalar, comps[2]/scalar);
  }
  inline void operator *= (float scalar) {
    comps[0] *= scalar; 
    comps[1] *= scalar; 
    comps[2] *= scalar;
  }
  inline void operator /= (float scalar) {
    assert(scalar>0.00000000f);
    comps[0] /= scalar; 
    comps[1] /= scalar; 
    comps[2] /= scalar;
  }

  inline vec3 normalize() const {
    vec3 v;
    v[0] = comps[0] / length();
    v[1] = comps[1] / length();
    v[2] = comps[2] / length();
    return v;
  }


  inline float length() const{
    return (sqrtf((comps[0]*comps[0])+(comps[1]*comps[1])+(comps[2]*comps[2])));
  }


  inline float squared_length() const {
    return comps[0] * comps[0] + comps[1] * comps[1] + comps[2] * comps[2];
  }

  inline void make_it_unit() {
    
    comps[0] /= length();
    comps[1] /= length();
    comps[2] /= length();

  }

  inline vec3& make_it_zero() {
    comps[0] = comps[1] = comps[2] = 0;
    return *this;
  }
  inline float dot(const vec3& other) const {
    return ((comps[0]*other[0])+(comps[1]*other[1])+(comps[2]*other[2]));
  }
  inline vec3 cross(const vec3& other) const {
    return vec3(
    comps[1]*other[2]+comps[2]*other[1],
      comps[2]*other[0]+comps[0]*other[2],
      comps[0]*other[1]+comps[1]*other[0]
    );
  }
  //checks 
  bool check_if_zero() const{
    //typecasting the floats to int since its hard to compare two floats against each other
    bool result = (((int)comps[0]) == 0 && ((int)comps[1]) == 0 && ((int)comps[2]) == 0);
    return result;
  }
  //IO
  friend std::ostream& operator << (std::ostream& cout, const vec3& other);
private:
  float comps[3]; //x,y & z or r,g & b
};


//non-member inline operators
inline vec3 operator * (const vec3& v,float scalar) {
  return vec3(v.x() * scalar, v.y() * scalar, v.z() * scalar);
}

inline vec3 operator * (float scalar, const vec3& v) {
  return vec3(v.x() * scalar, v.y() * scalar, v.z() * scalar);
}


inline float dot(const vec3& v1, const vec3& v2) {
  return (v1.x()*v2.x() + v1.y()*v2.y()+v1.z()*v2.z());
}
#endif