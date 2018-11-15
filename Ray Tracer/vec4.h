#ifndef INC_VEC4_H
#define INC_VEC4_H

#include<nmmintrin.h>

_declspec(align(16))
struct vec4
{
		__m128 vec;
	
	//construction
	vec4();
	vec4(float x, float y, float z, float w = 1.0f);
	explicit vec4(float n);
	vec4(const vec4& other);
	vec4& operator = (const vec4& other);
	//destruction
	~vec4();

	inline float operator[](int index) const
	{
		float result[4];
		_mm_store_ps(result,vec);
		return result[index];
	}

	inline void dot(const vec4& other,float& result)
	{
		__m128 dotResult = _mm_dp_ps(vec, other.vec, 0x7F);
		_mm_store_ps(&result,dotResult);
	}

	inline void make_it_unit()
	{
		__m128 selfDot = _mm_dp_ps(vec, vec, 0x7F);
		__m128 sqrtResult = _mm_rsqrt_ps(selfDot);
		vec = _mm_mul_ps(vec, sqrtResult);
	}

	inline void normalize(vec4& other)
	{
		__m128 selfDot = _mm_dp_ps(vec, vec, 0x7F);
		__m128 sqrtResult = _mm_rsqrt_ps(selfDot);
		other.vec = _mm_mul_ps(vec, sqrtResult);
	}

	inline float length()
	{
		__m128 selfDot = _mm_dp_ps(vec, vec, 0x7F);
		__m128 sqrtResult = _mm_sqrt_ps(selfDot);
		float result;
		_mm_store_ss(&result, sqrtResult);
		return result;
	}

	//vec4-vec4 arithmetic operations
	inline void add (const vec4& other,vec4& result) const{
		result.vec = _mm_add_ps(vec, other.vec);
		}

	inline void sub(const vec4& other, vec4& result) const {
		result.vec = _mm_sub_ps(vec, other.vec);
	}


	//vec4-vec4 arithmetic operations
	inline void operator += (const vec4& other) {
		vec = _mm_add_ps(vec, other.vec);
	}


	inline void mul (const vec4& other,vec4& result) const {
		result.vec = _mm_mul_ps(vec, other.vec);
	}

	inline void mul(float scalar, vec4& result) const
	{
		__m128 _scalar = _mm_set_ps(scalar, scalar, scalar, scalar);
		result.vec = _mm_mul_ps(vec, _scalar);
	}

	inline void div (const vec4& other,vec4& result) const {
		result.vec = _mm_div_ps(vec, other.vec);
	}
	inline void operator *= (const vec4& other) {
		vec = _mm_mul_ps(vec, other.vec);
	}
	inline void operator /= (const vec4& other) {
		vec = _mm_div_ps(vec, other.vec);
	}
	//vec4-scalar * & /

	inline void div (float scalar,vec4& result) const {
		__m128 _scalar = _mm_set_ps(scalar, scalar, scalar, scalar);
		result.vec = _mm_div_ps(vec, _scalar);
	}
	inline void operator *= (float scalar) {
		__m128 _scalar = _mm_set_ps(scalar, scalar, scalar, scalar);
		vec = _mm_mul_ps(vec, _scalar);
	}
	inline void operator /= (float scalar) {
		__m128 _scalar = _mm_set_ps(scalar, scalar, scalar, scalar);
		vec = _mm_div_ps(vec, _scalar);
	}



	inline void squared_length(float& result) const {
		__m128 dotResult = _mm_dp_ps(vec, vec, 0x7F);
		_mm_store_ss(&result, dotResult);
	}

	inline vec4& make_itzero() {
		vec = _mm_set_ps(0.0f, 0.0f, 0.0f, 0.0f);
	}


	//checks 
	bool check_ifzero() const {

	}
};

//non-member inline operators

inline void dot(const vec4& v1, const vec4& v2,float& result) {
	__m128 dotResult = _mm_dp_ps(v1.vec, v2.vec, 0x7F);
	_mm_store_ss(&result, dotResult);
}


#endif
