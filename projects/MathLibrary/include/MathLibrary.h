#pragma once
#include <cmath>
#include <string>

class vec2
{
public:
#pragma warning(push)
#pragma warning(disable : 4201)
	union {
		struct
		{
			float m_X;
			float m_Y;
		};
		float v[2];
	};
#pragma warning(pop)

public:
	vec2();
	vec2(float a_X, float a_Y);

	vec2& operator=(const vec2& rhs);

	vec2& operator+=(const vec2 &a_Rhs);
	vec2& operator-=(const vec2 &a_Rhs);
	vec2& operator*=(float a_Rhs);
	vec2& operator/=(float a_Rhs);

	float operator[](int a_Index) const;
	float& operator[](int a_Index);

	vec2& normalize();

	inline float dot(vec2 const& a_Rhs) const;
	inline float length() const;
	inline float squaredLength() const;
};

vec2 operator+(const vec2 &a_Lhs, const vec2 &a_Rhs);
vec2 operator-(const vec2 &a_Lhs, const vec2 &a_Rhs);
vec2 operator*(const vec2 &a_Lhs, float a_Rhs);
vec2 operator/(const vec2 &a_Lhs, float a_Rhs);
bool operator==(const vec2 &a_Lhs, const vec2 &a_Rhs);
bool operator!=(const vec2 &a_Lhs, const vec2 &a_Rhs);

class vec3 {
public:
#pragma warning(push)
#pragma warning(disable : 4201)
	union {
		struct
		{
			float m_X;
			float m_Y;
			float m_Z;
		};
		float v[3];
	};
#pragma warning(pop)

public:
	vec3();
	vec3(float a_X, float a_Y, float a_Z);

	vec3& operator=(const vec3& rhs);

	vec3& operator+=(const vec3 &a_Rhs);
	vec3& operator-=(const vec3 &a_Rhs);
	vec3& operator*=(float a_Rhs);
	vec3& operator/=(float a_Rhs);

	float operator[](int a_Index) const;
	float& operator[](int a_Index);

	vec3& normalize();

	inline float dot(vec3 const& a_Rhs) const;
	inline vec3 cross(vec3 const& a_Rhs) const;
	inline float length() const;
	inline float squaredLength() const;
};

vec3 operator+(const vec3 &a_Lhs, const vec3 &a_Rhs);
vec3 operator-(const vec3 &a_Lhs, const vec3 &a_Rhs);
vec3 operator*(const vec3 &a_Lhs, float a_Rhs);
vec3 operator/(const vec3 &a_Lhs, float a_Rhs);
bool operator==(const vec3 &a_Lhs, const vec3 &a_Rhs);
bool operator!=(const vec3 &a_Lhs, const vec3 &a_Rhs);

class vec4 {
public:
#pragma warning(push)
#pragma warning(disable : 4201)
	union {
		struct
		{
			float m_X;
			float m_Y;
			float m_Z;
			float m_T;
		};
		float v[4];
	};
#pragma warning(pop)

public:
	vec4(float a_X, float a_Y, float a_Z, float a_T);
	vec4(const vec3 &vec, float a_T);
};

class mat4
{
public:
#pragma warning(push)
#pragma warning(disable : 4201)
	union {
		struct
		{
			float m_00; float m_01; float m_02; float m_03;
			float m_10; float m_11; float m_12; float m_13;
			float m_20; float m_21; float m_22; float m_23;
			float m_30; float m_31; float m_32; float m_33;
		};
		float m[16];
	};
#pragma warning(pop)

public:
	mat4();
	mat4(vec3 a_Row0, float a_03,
		vec3 a_Row1, float a_13,
		vec3 a_Row2, float a_23,
		vec3 a_Row3, float a_33);
	mat4(float a_00, float a_01, float a_02, float a_03,
		float a_10, float a_11, float a_12, float a_13,
		float a_20, float a_21, float a_22, float a_23,
		float a_30, float a_31, float a_32, float a_33);

	mat4& operator=(const mat4& rhs);

	mat4& operator+=(const mat4 &a_Rhs);
	mat4& operator-=(const mat4 &a_Rhs);
	mat4& operator*=(const mat4 &a_Rhs);

	float determinant() const;
	inline mat4& inverse();

	static mat4 identity();
	static mat4 rotateX(float a_Radians);
	static mat4 rotateY(float a_Radians);
	static mat4 rotateZ(float a_Radians);
	static mat4 scale(const vec3 &a_Scale);
	static mat4 translate(const vec3 &a_Translation);

	static mat4 lookat(const vec3 &a_Eye, const vec3 &a_Center, const vec3 &a_Up);
	static mat4 projection(float a_FovY, float a_AspectRatio, float a_Near, float a_Far);
};

mat4 operator+(const mat4 &a_Lhs, const mat4 &a_Rhs);
mat4 operator-(const mat4 &a_Lhs, const mat4 &a_Rhs);
mat4 operator*(const mat4 &a_Lhs, const mat4 &a_Rhs);
// Assumes to use translation
vec3 operator*(const mat4 &a_Lhs, const vec3 &a_Rhs);
vec3 operator*(const mat4 &a_Lhs, const vec4 &a_Rhs);

// My functions
float createCofactor(const float*, const int, const int, const int);
float calcDet(const float *mat, const int dimension);
inline float lerp(float a, float b, float t);
template <typename T>
inline T clamp(const T value, const T min, const T max);
inline float radToDegrees(const float radians);
inline float degreesToRad(const float degrees);
inline vec3 getColor(const std::string &colorString);
constexpr float pi = 3.14f;

// All function declerations
// ------------------------------------------------

inline float lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

template <typename T>
inline T clamp(const T value, const T min, const T max)
{
	return value < min ? min : value > max ? max : value;
}

inline float radToDegrees(const float radians)
{
	return radians / pi * 180.f;
}

inline float degreesToRad(const float degrees)
{
	return degrees / 180.f * pi;
}

// colorString is RGB 
inline vec3 getColor(const std::string &colorString)
{
	// Convert hexadecimal string to unsigned integer

	const std::string hexString = colorString;
	const unsigned color = std::stoul(hexString, nullptr, 16);

	const unsigned rMask = 0xFF0000;
	const unsigned gMask = 0x00FF00;
	const unsigned bMask = 0x0000FF;

	const unsigned r = (color & rMask) >> (4 * 4);
	const unsigned g = (color & gMask) >> (2 * 4);
	const unsigned b = color & bMask;

	const unsigned char red = static_cast<unsigned char>(r);
	const unsigned char green = static_cast<unsigned char>(g);
	const unsigned char blue = static_cast<unsigned char>(b);

	return vec3(red, green, blue);
}

// Vec2

inline float vec2::dot(vec2 const& a_Rhs) const
{
	return 
		v[0] * a_Rhs[0] + 
		v[1] * a_Rhs[1];
}

inline float vec2::length() const
{
	return std::sqrtf(
		m_X * m_X + 
		m_Y * m_Y);
}

inline float vec2::squaredLength() const
{
	return 
		m_X * m_X + 
		m_Y * m_Y;
}

// Vec3

inline float vec3::dot(vec3 const& a_Rhs) const
{
	return
		v[0] * a_Rhs[0] +
		v[1] * a_Rhs[1] +
		v[2] * a_Rhs[2];
}

inline vec3 vec3::cross(vec3 const& a_Rhs) const
{
	return vec3(
		v[1] * a_Rhs[2] - v[2] * a_Rhs[1],
		v[2] * a_Rhs[0] - v[0] * a_Rhs[2],
		v[0] * a_Rhs[1] - v[1] * a_Rhs[0]);
}

inline float vec3::length() const
{
	return std::sqrtf(
		m_X * m_X +
		m_Y * m_Y +
		m_Z * m_Z);
}

inline float vec3::squaredLength() const
{
	return
		m_X * m_X +
		m_Y * m_Y +
		m_Z * m_Z;
}

// Mat4

inline mat4 & mat4::inverse()
{
	mat4 orig = *this;
	const float det = calcDet(m, 4);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			// By flipping the i and j we are implicitly creating a transpose
			m[j + i * 4] = createCofactor(orig.m, 4, j, i) / det;

	return *this;
}