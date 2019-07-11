#include "MathLibrary.h"

#include <cassert>

#pragma region vec2
vec2::vec2():
	vec2(0,0)
{
}

vec2::vec2(float a_X, float a_Y) :
	m_X(a_X),
	m_Y(a_Y)
{
}

vec2 & vec2::operator=(const vec2 & a_Rhs)
{
	for (int i = 0; i < 2; i++) v[i] = a_Rhs[i];
	return *this;
}

vec2 & vec2::operator+=(const vec2 & a_Rhs)
{
	for (int i = 0; i < 2; i++) v[i] += a_Rhs[i];
	return *this;
}

vec2 & vec2::operator-=(const vec2 & a_Rhs)
{
	for (int i = 0; i < 2; i++) v[i] -= a_Rhs[i];
	return *this;
}

vec2 & vec2::operator*=(float a_Rhs)
{
	for (int i = 0; i < 2; i++) v[i] *= a_Rhs;
	return *this;
}

vec2 & vec2::operator/=(float a_Rhs)
{
	assert(a_Rhs != 0 && "Can't divide vector by zero");
	for (int i = 0; i < 2; i++) v[i] /= a_Rhs;
	return *this;
}

float vec2::operator[](int a_Index) const
{
	assert(a_Index >= 0 && a_Index <= 1 && "Vector subscript out of range");
	return v[a_Index];
}

float & vec2::operator[](int a_Index)
{
	assert(a_Index >= 0 && a_Index <= 1 && "Vector subscript out of range");
	return v[a_Index];
}

vec2 & vec2::normalize()
{
	return *this /= length();
}

vec2 operator+(const vec2 & a_Lhs, const vec2 & a_Rhs)
{
	return vec2(
		a_Lhs[0] + a_Rhs[0], 
		a_Lhs[1] + a_Rhs[1]);
}

vec2 operator-(const vec2 & a_Lhs, const vec2 & a_Rhs)
{
	return vec2(
		a_Lhs[0] - a_Rhs[0], 
		a_Lhs[1] - a_Rhs[1]);
}

vec2 operator*(const vec2 & a_Lhs, float a_Rhs)
{
	return vec2(
		a_Lhs[0] * a_Rhs, 
		a_Lhs[1] * a_Rhs);
}

vec2 operator/(const vec2 & a_Lhs, float a_Rhs)
{
	assert(a_Rhs != 0 && "Can't divide by zero");
	return vec2(
		a_Lhs[0] / a_Rhs, 
		a_Lhs[1] / a_Rhs);
}

bool operator==(const vec2 & a_Lhs, const vec2 & a_Rhs)
{
	return (
		a_Lhs[0] == a_Rhs[0] && 
		a_Lhs[1] == a_Rhs[1]);
}

bool operator!=(const vec2 & a_Lhs, const vec2 & a_Rhs)
{
	return !(a_Lhs == a_Rhs);
}
#pragma endregion

#pragma region vec3
vec3::vec3() :
	vec3(0, 0, 0)
{
}

vec3::vec3(float a_X, float a_Y, float a_Z) :
	m_X(a_X),
	m_Y(a_Y),
	m_Z(a_Z)
{
}

vec3 & vec3::operator=(const vec3 & a_Rhs)
{
	for (int i = 0; i < 3; i++) v[i] = a_Rhs[i];
	return *this;
}

vec3 & vec3::operator+=(const vec3 & a_Rhs)
{
	for (int i = 0; i < 3; i++) v[i] += a_Rhs[i];
	return *this;
}

vec3 & vec3::operator-=(const vec3 & a_Rhs)
{
	for (int i = 0; i < 3; i++) v[i] -= a_Rhs[i];
	return *this;
}

vec3 & vec3::operator*=(float a_Rhs)
{
	for (int i = 0; i < 3; i++) v[i] *= a_Rhs;
	return *this;
}

vec3 & vec3::operator/=(float a_Rhs)
{
	assert(a_Rhs != 0 && "Can't divide vector by zero");
	for (int i = 0; i < 3; i++) v[i] /= a_Rhs;
	return *this;
}

float vec3::operator[](int a_Index) const
{
	assert(a_Index >= 0 && a_Index <= 2 && "Vector subscript out of range");
	return v[a_Index];
}

float & vec3::operator[](int a_Index)
{
	assert(a_Index >= 0 && a_Index <= 2 && "Vector subscript out of range");
	return v[a_Index];
}

vec3 & vec3::normalize()
{
	return *this /= length();
}

vec3 operator+(const vec3 & a_Lhs, const vec3 & a_Rhs)
{
	return vec3(
		a_Lhs[0] + a_Rhs[0], 
		a_Lhs[1] + a_Rhs[1], 
		a_Lhs[2] + a_Rhs[2]);
}

vec3 operator-(const vec3 & a_Lhs, const vec3 & a_Rhs)
{
	return vec3(
		a_Lhs[0] - a_Rhs[0], 
		a_Lhs[1] - a_Rhs[1], 
		a_Lhs[2] - a_Rhs[2]);
}

vec3 operator*(const vec3 & a_Lhs, float a_Rhs)
{
	return vec3(
		a_Lhs[0] * a_Rhs, 
		a_Lhs[1] * a_Rhs, 
		a_Lhs[2] * a_Rhs);
}

vec3 operator/(const vec3 & a_Lhs, float a_Rhs)
{
	assert(a_Rhs != 0 && "Can't divide vector by zero");
	return vec3(
		a_Lhs[0] / a_Rhs, 
		a_Lhs[1] / a_Rhs, 
		a_Lhs[2] / a_Rhs);
}

bool operator==(const vec3 & a_Lhs, const vec3 & a_Rhs)
{
	return (
		a_Lhs[0] == a_Rhs[0] && 
		a_Lhs[1] == a_Rhs[1] && 
		a_Lhs[2] == a_Rhs[2]);
}

bool operator!=(const vec3 & a_Lhs, const vec3 & a_Rhs)
{
	return !(a_Lhs == a_Rhs);
}

#pragma endregion

#pragma region mat4

mat4::mat4():
	mat4(0.f, 0.f, 0.f, 0.f,
		 0.f, 0.f, 0.f, 0.f,
		 0.f, 0.f, 0.f, 0.f,
		 0.f, 0.f, 0.f, 0.f)
{
}

mat4::mat4(vec3 a_Row0, float a_03, 
		   vec3 a_Row1, float a_13, 
		   vec3 a_Row2, float a_23, 
		   vec3 a_Row3, float a_33):
	mat4(a_Row0[0], a_Row0[1], a_Row0[2], a_03,
		 a_Row1[0], a_Row1[1], a_Row1[2], a_13,
		 a_Row2[0], a_Row2[1], a_Row2[2], a_23,
		 a_Row3[0], a_Row3[1], a_Row3[2], a_33)
{
}

mat4::mat4(float a_00, float a_01, float a_02, float a_03, 
		   float a_10, float a_11, float a_12, float a_13, 
		   float a_20, float a_21, float a_22, float a_23, 
		   float a_30, float a_31, float a_32, float a_33):
	    m_00(a_00), m_01(a_01), m_02(a_02), m_03(a_03),
		m_10(a_10), m_11(a_11), m_12(a_12), m_13(a_13),
		m_20(a_20), m_21(a_21), m_22(a_22), m_23(a_23),
		m_30(a_30), m_31(a_31), m_32(a_32), m_33(a_33)
{
}

mat4 & mat4::operator=(const mat4 & a_Rhs)
{
	for (int i = 0; i < 16; i++) m[i] = a_Rhs.m[i];
	return *this;
}

mat4 & mat4::operator+=(const mat4 & a_Rhs)
{
	for (int i = 0; i < 16; i++) m[i] += a_Rhs.m[i];
	return *this;
}

mat4 & mat4::operator-=(const mat4 & a_Rhs)
{
	for (int i = 0; i < 16; i++) m[i] -= a_Rhs.m[i];
	return *this;
}

mat4 & mat4::operator*=(const mat4 & a_Rhs)
{
	mat4 returner;

	for(int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
		{
			float dot = 0;
			for (int i = 0; i < 4; i++)
				dot += m[y * 4 + i] * a_Rhs.m[x + i * 4];

			returner.m[y * 4 + x] = dot;
		}

	*this = returner;
	return *this;
}

float calcDet(const float *mat, const int dimension)
{
	if (dimension == 2)
	{
		return mat[0] * mat[3] - mat[2] * mat[1];
	}

	float det = 0;
	for (int c = 0; c < dimension; c++)
	{
		det += mat[c] * createCofactor(mat, dimension, 0, c);
	}
	return det;
}

float createCofactor(const float *mat, const int dimension, const int i, const int j)
{
	if (dimension == 2)
	{
		calcDet(mat, dimension);
	}
	
	// Create subsequent matrix w/o i and j
	float *submat = new float[(dimension -1) * (dimension -1)];
	int submatIndex = 0;
	for (int r = 0; r < dimension; r++)
	{
		if (r == i)
			continue;

		for (int c = 0; c < dimension; c++)
		{
			if (c == j)
				continue;

			submat[submatIndex++] = mat[c + r * dimension];
		}
	}

	// The cofactor of a element is the determinant of the matrix w/o the row and col
	float det = calcDet(submat, dimension - 1);
	if ((i + j) % 2 == 1)
		det = -det;

	delete[] submat;
	return det;
}

float mat4::determinant() const
{
	return calcDet(m, 4);
}

mat4 mat4::identity()
{
	return mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

mat4 mat4::rotateX(float a_Radians)
{
	mat4 mat = mat4::identity();
	mat.m_11 = std::cos(a_Radians);
	mat.m_21 = std::sin(a_Radians);
	mat.m_12 = -std::sin(a_Radians);
	mat.m_22 = std::cos(a_Radians);
	return mat;
}

mat4 mat4::rotateY(float a_Radians)
{
	mat4 mat = mat4::identity();
	mat.m_00 = std::cos(a_Radians);
	mat.m_20 = -std::sin(a_Radians);
	mat.m_02 = std::sin(a_Radians);
	mat.m_22 = std::cos(a_Radians);
	return mat;
}

mat4 mat4::rotateZ(float a_Radians)
{
	mat4 mat = mat4::identity();
	mat.m_00 = std::cos(a_Radians);
	mat.m_10 = std::sin(a_Radians);
	mat.m_01 = -std::sin(a_Radians);
	mat.m_11 = std::cos(a_Radians);
	return mat;
}

mat4 mat4::scale(const vec3 & a_Scale)
{
	mat4 mat = mat4::identity();
	for (int i = 0; i < 3; i++)
		mat.m[i + i * 4] = a_Scale[i];
	return mat;
}

mat4 mat4::translate(const vec3 & a_Translation)
{
	mat4 mat = mat4::identity();
	for (int i = 0; i < 3; i++)
		mat.m[3 + i * 4] = a_Translation[i];
	return mat;
}

mat4 mat4::lookat(const vec3 & a_Eye, const vec3 & a_Center, const vec3 & a_Up)
{
	const vec3 z = (a_Eye - a_Center).normalize();
	const vec3 x = a_Up.cross(z).normalize();
	const vec3 y = z.cross(x).normalize();

	mat4 translation(
		1.f,   0,   0, a_Eye[0],
		  0, 1.f,   0, a_Eye[1],
		  0,   0, 1.f, a_Eye[2],
		0.f, 0.f, 0.f,     1.f);

	mat4 orientation(
		x[0], x[1], x[2], 0.f,
		y[0], y[1], y[2], 0.f,
		z[0], z[1], z[2], 0.f,
		 0.f,  0.f,  0.f, 1.f);

	// Column major
	return translation * orientation;

	// Row major
	//return mat4(
	//		x[0],	  y[0],		z[0], 0,
	//		x[1],	  y[1],		z[1], 0,
	//		x[2],	  y[2],		z[2], 0,
	//	a_Eye[0], a_Eye[1], a_Eye[2], 1);
}		
mat4 mat4::projection(float a_FovY, float a_AspectRatio, float a_Near, float a_Far)
{
	return mat4();
}

mat4 operator+(const mat4 & a_Lhs, const mat4 & a_Rhs)
{
	mat4 returner{ a_Lhs };
	returner += a_Rhs;
	return returner;
}

mat4 operator-(const mat4 & a_Lhs, const mat4 & a_Rhs)
{
	mat4 returner{ a_Lhs };
	returner -= a_Rhs;
	return returner;
}

mat4 operator*(const mat4 & a_Lhs, const mat4 & a_Rhs)
{
	mat4 returner{ a_Lhs };
	returner *= a_Rhs;
	return returner;
}

vec3 operator*(const mat4 & a_Lhs, const vec3 & a_Rhs)
{
	return a_Lhs * vec4(a_Rhs[0], a_Rhs[1], a_Rhs[2], 1);
}

vec3 operator*(const mat4 & a_Lhs, const vec4 & a_Rhs)
{
	const float vec[4]{ a_Rhs.v[0],a_Rhs.v[1],a_Rhs.v[2], a_Rhs.v[3] };
	float returner[4];

	for (int r = 0; r < 4; r++)
	{
		// Create the 2 vec that will be dotted
		float operation[4];
		for (int c = 0; c < 4; c++)
			operation[c] = a_Lhs.m[c + r * 4];

		// Dot the 2 vec and put result in correct row
		float dot = 0;
		for (int i = 0; i < 4; i++)
			dot += vec[i] * operation[i];
		returner[r] = dot;
	}

	return vec3(returner[0], returner[1], returner[2]);
}

#pragma endregion

vec4::vec4(float a_X, float a_Y, float a_Z, float a_T):
	m_X(a_X),
	m_Y(a_Y),
	m_Z(a_Z),
	m_T(a_T)
{
}

vec4::vec4(const vec3 & vec, float a_T):
	vec4(vec.v[0],vec.v[1],vec.v[2],a_T)
{
}
