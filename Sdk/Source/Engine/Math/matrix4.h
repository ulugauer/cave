/*
Copyright (c) <2017> <Udo Lugauer>
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE
*/
#pragma once

/// @file matrix4.h
///       Handles material assets

#include "vector3.h"
#include "vector4.h"

/** \addtogroup engine
*  @{
*
*/

namespace cave
{


/**
* @brief Matrix 4x4 implementation
*/
template<typename T>
class Matrix4
{
public:
	float _m[4][4];		///< matrix entries

	/** default constructor */
	Matrix4<T>() {};

	/** 
	* @brief constructor
	*
	* @param forceInit	Set to identity matrix
	*
	*/
	Matrix4<T>( bool forceInit) 
	{
		if (forceInit)
			SetIdentity();
	};

	/** destructor */
	~Matrix4<T>() {};

	/**
	* @brief Set Identity matrix
	*/
	void SetIdentity(void)
	{
		_m[0][0] = _m[1][1] = _m[2][2] = _m[3][3] = 1.0f;
		_m[0][1] = _m[0][2] = _m[0][3] =
		_m[1][0] = _m[1][2] = _m[1][3] =
		_m[2][0] = _m[2][1] = _m[2][3] =
		_m[3][0] = _m[3][1] = _m[3][2] = 0.0f;
	}

	/**
	* @brief Multiply two matrices
	*
	* @param m2		Does a this->_m * m2
	* 
	* @result return new matrix
	*/
	Matrix4<T> operator *(const Matrix4<T>& m2)
	{
		Matrix4<T> result;

		result._m[0][0] = _m[0][0] * m2._m[0][0] + _m[1][0] * m2._m[0][1] + _m[2][0] * m2._m[0][2] + _m[3][0] * m2._m[0][3];
		result._m[1][0] = _m[0][0] * m2._m[1][0] + _m[1][0] * m2._m[1][1] + _m[2][0] * m2._m[1][2] + _m[3][0] * m2._m[1][3];
		result._m[2][0] = _m[0][0] * m2._m[2][0] + _m[1][0] * m2._m[2][1] + _m[2][0] * m2._m[2][2] + _m[3][0] * m2._m[2][3];
		result._m[3][0] = _m[0][0] * m2._m[3][0] + _m[1][0] * m2._m[3][1] + _m[2][0] * m2._m[3][2] + _m[3][0] * m2._m[3][3];
		result._m[0][1] = _m[0][1] * m2._m[0][0] + _m[1][1] * m2._m[0][1] + _m[2][1] * m2._m[0][2] + _m[3][1] * m2._m[0][3];
		result._m[1][1] = _m[0][1] * m2._m[1][0] + _m[1][1] * m2._m[1][1] + _m[2][1] * m2._m[1][2] + _m[3][1] * m2._m[1][3];
		result._m[2][1] = _m[0][1] * m2._m[2][0] + _m[1][1] * m2._m[2][1] + _m[2][1] * m2._m[2][2] + _m[3][1] * m2._m[2][3];
		result._m[3][1] = _m[0][1] * m2._m[3][0] + _m[1][1] * m2._m[3][1] + _m[2][1] * m2._m[3][2] + _m[3][1] * m2._m[3][3];
		result._m[0][2] = _m[0][2] * m2._m[0][0] + _m[1][2] * m2._m[0][1] + _m[2][2] * m2._m[0][2] + _m[3][2] * m2._m[0][3];
		result._m[1][2] = _m[0][2] * m2._m[1][0] + _m[1][2] * m2._m[1][1] + _m[2][2] * m2._m[1][2] + _m[3][2] * m2._m[1][3];
		result._m[2][2] = _m[0][2] * m2._m[2][0] + _m[1][2] * m2._m[2][1] + _m[2][2] * m2._m[2][2] + _m[3][2] * m2._m[2][3];
		result._m[3][2] = _m[0][2] * m2._m[3][0] + _m[1][2] * m2._m[3][1] + _m[2][2] * m2._m[3][2] + _m[3][2] * m2._m[3][3];
		result._m[0][3] = _m[0][3] * m2._m[0][0] + _m[1][3] * m2._m[0][1] + _m[2][3] * m2._m[0][2] + _m[3][3] * m2._m[0][3];
		result._m[1][3] = _m[0][3] * m2._m[1][0] + _m[1][3] * m2._m[1][1] + _m[2][3] * m2._m[1][2] + _m[3][3] * m2._m[1][3];
		result._m[2][3] = _m[0][3] * m2._m[2][0] + _m[1][3] * m2._m[2][1] + _m[2][3] * m2._m[2][2] + _m[3][3] * m2._m[2][3];
		result._m[3][3] = _m[0][3] * m2._m[3][0] + _m[1][3] * m2._m[3][1] + _m[2][3] * m2._m[3][2] + _m[3][3] * m2._m[3][3];
		
		return result;
	}
};

typedef Matrix4<float> Matrix4f;	///< type specialization
typedef Matrix4<double> Matrix4d;	///< type specialization


/**
* @brief Multiply two matrices
*
* @param m1		Matrix A 
* @param m2		Matrix B
* 
* @result return matrix A * B
*/
template<typename T>
Matrix4<T> Multiply(const Matrix4<T>& m1, const Matrix4<T>& m2)
{
	Matrix4<T> result;

	result._m[0][0] = m1._m[0][0] * m2._m[0][0] + m1._m[1][0] * m2._m[0][1] + m1._m[2][0] * m2._m[0][2] + m1._m[3][0] * m2._m[0][3];
	result._m[1][0] = m1._m[0][0] * m2._m[1][0] + m1._m[1][0] * m2._m[1][1] + m1._m[2][0] * m2._m[1][2] + m1._m[3][0] * m2._m[1][3];
	result._m[2][0] = m1._m[0][0] * m2._m[2][0] + m1._m[1][0] * m2._m[2][1] + m1._m[2][0] * m2._m[2][2] + m1._m[3][0] * m2._m[2][3];
	result._m[3][0] = m1._m[0][0] * m2._m[3][0] + m1._m[1][0] * m2._m[3][1] + m1._m[2][0] * m2._m[3][2] + m1._m[3][0] * m2._m[3][3];
	result._m[0][1] = m1._m[0][1] * m2._m[0][0] + m1._m[1][1] * m2._m[0][1] + m1._m[2][1] * m2._m[0][2] + m1._m[3][1] * m2._m[0][3];
	result._m[1][1] = m1._m[0][1] * m2._m[1][0] + m1._m[1][1] * m2._m[1][1] + m1._m[2][1] * m2._m[1][2] + m1._m[3][1] * m2._m[1][3];
	result._m[2][1] = m1._m[0][1] * m2._m[2][0] + m1._m[1][1] * m2._m[2][1] + m1._m[2][1] * m2._m[2][2] + m1._m[3][1] * m2._m[2][3];
	result._m[3][1] = m1._m[0][1] * m2._m[3][0] + m1._m[1][1] * m2._m[3][1] + m1._m[2][1] * m2._m[3][2] + m1._m[3][1] * m2._m[3][3];
	result._m[0][2] = m1._m[0][2] * m2._m[0][0] + m1._m[1][2] * m2._m[0][1] + m1._m[2][2] * m2._m[0][2] + m1._m[3][2] * m2._m[0][3];
	result._m[1][2] = m1._m[0][2] * m2._m[1][0] + m1._m[1][2] * m2._m[1][1] + m1._m[2][2] * m2._m[1][2] + m1._m[3][2] * m2._m[1][3];
	result._m[2][2] = m1._m[0][2] * m2._m[2][0] + m1._m[1][2] * m2._m[2][1] + m1._m[2][2] * m2._m[2][2] + m1._m[3][2] * m2._m[2][3];
	result._m[3][2] = m1._m[0][2] * m2._m[3][0] + m1._m[1][2] * m2._m[3][1] + m1._m[2][2] * m2._m[3][2] + m1._m[3][2] * m2._m[3][3];
	result._m[0][3] = m1._m[0][3] * m2._m[0][0] + m1._m[1][3] * m2._m[0][1] + m1._m[2][3] * m2._m[0][2] + m1._m[3][3] * m2._m[0][3];
	result._m[1][3] = m1._m[0][3] * m2._m[1][0] + m1._m[1][3] * m2._m[1][1] + m1._m[2][3] * m2._m[1][2] + m1._m[3][3] * m2._m[1][3];
	result._m[2][3] = m1._m[0][3] * m2._m[2][0] + m1._m[1][3] * m2._m[2][1] + m1._m[2][3] * m2._m[2][2] + m1._m[3][3] * m2._m[2][3];
	result._m[3][3] = m1._m[0][3] * m2._m[3][0] + m1._m[1][3] * m2._m[3][1] + m1._m[2][3] * m2._m[3][2] + m1._m[3][3] * m2._m[3][3];

	return result;
}

/**
* @brief Create a orthograpic projection matrix left handed
*
* @param width	dimension
* @param height	dimension
* @param znear	Z near boundary
* @param zfar	Z far boundary
*
* @result return left handed ortho matrix
*/
template<typename T>
Matrix4<T> OrthoLH(T width, T height, T znear, T zfar)
{
	Matrix4<T> result(true);

	result._m[0][0] = 2 / width;
	result._m[1][1] = 2 / height;
	result._m[2][2] = 1 / (zfar - znear);
	result._m[3][2] = -znear / (zfar - znear);

	return result;
}

/**
* @brief Create a orthograpic projection matrix right handed
*
* @param width	dimension
* @param height	dimension
* @param znear	Z near boundary
* @param zfar	Z far boundary
*
* @result return right handed ortho matrix
*/
template<typename T>
Matrix4<T> OrthoRH(T width, T height, T znear, T zfar)
{
	Matrix4<T> result(true);

	result._m[0][0] = 2 / width;
	result._m[1][1] = 2 / height;
	result._m[2][2] = 1 / (znear - zfar);
	result._m[3][2] = znear / (znear - zfar);

	return result;
}

/**
* @brief Create a perspective projection matrix left handed
*
* @param width	dimension
* @param height	dimension
* @param znear	Z near boundary
* @param zfar	Z far boundary
*
* @result return left handed perspecitve matrix
*/
template<typename T>
Matrix4<T> PerspectiveLH(T width, T height, T znear, T zfar)
{
	Matrix4<T> result(true);

	result._m[0][0] = 2 * znear / width;
	result._m[1][1] = 2 * znear / height;
	result._m[2][2] = zfar / (zfar - znear);
	result._m[3][2] = znear * zfar / (znear - zfar);
	result._m[2][3] = 1;
	result._m[3][3] = 0;

	return result;
}

/**
* @brief Create a perspective projection matrix right handed
*
* @param width	dimension
* @param height	dimension
* @param znear	Z near boundary
* @param zfar	Z far boundary
*
* @result return right handed perspecitve matrix
*/
template<typename T>
Matrix4<T> PerspectiveRH(T width, T height, T znear, T zfar)
{
	Matrix4<T> result(true);

	result._m[0][0] = 2 * znear / width;
	result._m[1][1] = 2 * znear / height;
	result._m[2][2] = zfar / (znear - zfar);
	result._m[3][2] = znear * zfar / (znear - zfar);
	result._m[2][3] = -1;
	result._m[3][3] = 0;

	return result;
}

/**
* @brief Create a look matrix left handed
*
* @param eye	eye position
* @param at		look at vector
* @param up		up vector
*
* @result return camera matrix in a left handed coordinate system
*/
template<typename T>
Matrix4<T> LookAtMatrixLH(const Vector3<T>& eye, const Vector3<T>& at,  const Vector3<T>& up)
{
	Matrix4<T> result(true);

	Vector3<T> zaxis = Normalize(at - eye);
	Vector3<T> xaxis = Normalize(CrossProduct(up, zaxis));
	Vector3<T> yaxis = CrossProduct(zaxis, xaxis);

	result._m[0][0] = xaxis._x;
	result._m[0][1] = yaxis._x;
	result._m[0][2] = zaxis._x;
	result._m[1][0] = xaxis._y;
	result._m[1][1] = yaxis._y;
	result._m[1][2] = zaxis._y;
	result._m[2][0] = xaxis._z;
	result._m[2][1] = yaxis._z;
	result._m[2][2] = zaxis._z;
	result._m[3][0] = -DotProduct(xaxis, eye);
	result._m[3][1] = -DotProduct(yaxis, eye);
	result._m[3][2] = -DotProduct(zaxis, eye);

	return result;
}

/**
* @brief Create a look matrix right handed
*
* @param eye	eye position
* @param at		look at vector
* @param up		up vector
*
* @result return camera matrix in a right handed coordinate system
*/
template<typename T>
Matrix4<T> LookAtMatrixRH(const Vector3<T>& eye, const Vector3<T>& at, const Vector3<T>& up)
{
	Matrix4<T> result(true);

	Vector3<T> zaxis = Normalize(eye - at);
	Vector3<T> xaxis = Normalize(CrossProduct(up, zaxis));
	Vector3<T> yaxis = CrossProduct(zaxis, xaxis);

	result._m[0][0] = xaxis._x;
	result._m[0][1] = yaxis._x;
	result._m[0][2] = zaxis._x;
	result._m[1][0] = xaxis._y;
	result._m[1][1] = yaxis._y;
	result._m[1][2] = zaxis._y;
	result._m[2][0] = xaxis._z;
	result._m[2][1] = yaxis._z;
	result._m[2][2] = zaxis._z;
	result._m[3][0] = DotProduct(xaxis, eye);
	result._m[3][1] = DotProduct(yaxis, eye);
	result._m[3][2] = DotProduct(zaxis, eye);

	return result;
}

/**
* @brief Create a transposed matrix
*
* @param A		Matrix
*
* @result return transposed matrix
*/
template<typename T>
Matrix4<T> Transpose(const Matrix4<T>& A)
{
	Matrix4<T> result(false);
	result._m[0][0] = A._m[0][0]; result._m[0][1] = A._m[1][0]; result._m[0][2] = A._m[2][0]; result._m[0][3] = A._m[3][0];
	result._m[1][0] = A._m[0][1]; result._m[1][1] = A._m[1][1]; result._m[1][2] = A._m[2][1]; result._m[1][3] = A._m[3][1];
	result._m[2][0] = A._m[0][2]; result._m[2][1] = A._m[1][2]; result._m[2][2] = A._m[2][2]; result._m[2][3] = A._m[3][2];
	result._m[3][0] = A._m[0][3]; result._m[3][1] = A._m[1][3]; result._m[3][2] = A._m[2][3]; result._m[3][3] = A._m[3][3];

	return result;
}

/**
* @brief Create a translation  matrix
*
* @param x	Translation in X
* @param y	Translation in Y
* @param z	Translation in Z
*
* @result return translation matrix
*/
template<typename T>
Matrix4<T> Translation(T x, T y, T z)
{
	Matrix4<T> result(true);
	result._m[3][0] = x;
	result._m[3][1] = y;
	result._m[3][2] = z;

	return result;
}

}

/** @}*/

