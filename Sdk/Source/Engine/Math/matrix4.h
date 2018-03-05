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
	template<typename T>
	Matrix4<T> operator *(const Matrix4<T>& m2)
	{
		Matrix4<T> result;

		result._m[0][0] = _m[0][0] * m2._m[0][0] + _m[1][0] * m2._m[0][1] + _m[2][0] * m2._m[0][2] + _m[3][0] * m2._m[0][3];
		result._m[0][1] = _m[0][0] * m2._m[1][0] + _m[1][0] * m2._m[1][1] + _m[2][0] * m2._m[1][2] + _m[3][0] * m2._m[1][3];
		result._m[0][2] = _m[0][0] * m2._m[2][0] + _m[1][0] * m2._m[2][1] + _m[2][0] * m2._m[2][2] + _m[3][0] * m2._m[2][3];
		result._m[0][3] = _m[0][0] * m2._m[3][0] + _m[1][0] * m2._m[3][1] + _m[2][0] * m2._m[3][2] + _m[3][0] * m2._m[3][3];
		result._m[1][0] = _m[0][1] * m2._m[0][0] + _m[1][1] * m2._m[0][1] + _m[2][1] * m2._m[0][2] + _m[3][1] * m2._m[0][3];
		result._m[1][1] = _m[0][1] * m2._m[1][0] + _m[1][1] * m2._m[1][1] + _m[2][1] * m2._m[1][2] + _m[3][1] * m2._m[1][3];
		result._m[1][2] = _m[0][1] * m2._m[2][0] + _m[1][1] * m2._m[2][1] + _m[2][1] * m2._m[2][2] + _m[3][1] * m2._m[2][3];
		result._m[1][3] = _m[0][1] * m2._m[3][0] + _m[1][1] * m2._m[3][1] + _m[2][1] * m2._m[3][2] + _m[3][1] * m2._m[3][3];
		result._m[2][0] = _m[0][2] * m2._m[0][0] + _m[1][2] * m2._m[0][1] + _m[2][2] * m2._m[0][2] + _m[3][2] * m2._m[0][3];
		result._m[2][1] = _m[0][2] * m2._m[1][0] + _m[1][2] * m2._m[1][1] + _m[2][2] * m2._m[1][2] + _m[3][2] * m2._m[1][3];
		result._m[2][2] = _m[0][2] * m2._m[2][0] + _m[1][2] * m2._m[2][1] + _m[2][2] * m2._m[2][2] + _m[3][2] * m2._m[2][3];
		result._m[2][3] = _m[0][2] * m2._m[3][0] + _m[1][2] * m2._m[3][1] + _m[2][2] * m2._m[3][2] + _m[3][2] * m2._m[3][3];
		result._m[3][0] = _m[0][3] * m2._m[0][0] + _m[1][3] * m2._m[0][1] + _m[2][3] * m2._m[0][2] + _m[3][3] * m2._m[0][3];
		result._m[3][1] = _m[0][3] * m2._m[1][0] + _m[1][3] * m2._m[1][1] + _m[2][3] * m2._m[1][2] + _m[3][3] * m2._m[1][3];
		result._m[3][2] = _m[0][3] * m2._m[2][0] + _m[1][3] * m2._m[2][1] + _m[2][3] * m2._m[2][2] + _m[3][3] * m2._m[2][3];
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
	result._m[0][1] = m1._m[0][0] * m2._m[1][0] + m1._m[1][0] * m2._m[1][1] + m1._m[2][0] * m2._m[1][2] + m1._m[3][0] * m2._m[1][3];
	result._m[0][2] = m1._m[0][0] * m2._m[2][0] + m1._m[1][0] * m2._m[2][1] + m1._m[2][0] * m2._m[2][2] + m1._m[3][0] * m2._m[2][3];
	result._m[0][3] = m1._m[0][0] * m2._m[3][0] + m1._m[1][0] * m2._m[3][1] + m1._m[2][0] * m2._m[3][2] + m1._m[3][0] * m2._m[3][3];
	result._m[1][0] = m1._m[0][1] * m2._m[0][0] + m1._m[1][1] * m2._m[0][1] + m1._m[2][1] * m2._m[0][2] + m1._m[3][1] * m2._m[0][3];
	result._m[1][1] = m1._m[0][1] * m2._m[1][0] + m1._m[1][1] * m2._m[1][1] + m1._m[2][1] * m2._m[1][2] + m1._m[3][1] * m2._m[1][3];
	result._m[1][2] = m1._m[0][1] * m2._m[2][0] + m1._m[1][1] * m2._m[2][1] + m1._m[2][1] * m2._m[2][2] + m1._m[3][1] * m2._m[2][3];
	result._m[1][3] = m1._m[0][1] * m2._m[3][0] + m1._m[1][1] * m2._m[3][1] + m1._m[2][1] * m2._m[3][2] + m1._m[3][1] * m2._m[3][3];
	result._m[2][0] = m1._m[0][2] * m2._m[0][0] + m1._m[1][2] * m2._m[0][1] + m1._m[2][2] * m2._m[0][2] + m1._m[3][2] * m2._m[0][3];
	result._m[2][1] = m1._m[0][2] * m2._m[1][0] + m1._m[1][2] * m2._m[1][1] + m1._m[2][2] * m2._m[1][2] + m1._m[3][2] * m2._m[1][3];
	result._m[2][2] = m1._m[0][2] * m2._m[2][0] + m1._m[1][2] * m2._m[2][1] + m1._m[2][2] * m2._m[2][2] + m1._m[3][2] * m2._m[2][3];
	result._m[2][3] = m1._m[0][2] * m2._m[3][0] + m1._m[1][2] * m2._m[3][1] + m1._m[2][2] * m2._m[3][2] + m1._m[3][2] * m2._m[3][3];
	result._m[3][0] = m1._m[0][3] * m2._m[0][0] + m1._m[1][3] * m2._m[0][1] + m1._m[2][3] * m2._m[0][2] + m1._m[3][3] * m2._m[0][3];
	result._m[3][1] = m1._m[0][3] * m2._m[1][0] + m1._m[1][3] * m2._m[1][1] + m1._m[2][3] * m2._m[1][2] + m1._m[3][3] * m2._m[1][3];
	result._m[3][2] = m1._m[0][3] * m2._m[2][0] + m1._m[1][3] * m2._m[2][1] + m1._m[2][3] * m2._m[2][2] + m1._m[3][3] * m2._m[2][3];
	result._m[3][3] = m1._m[0][3] * m2._m[3][0] + m1._m[1][3] * m2._m[3][1] + m1._m[2][3] * m2._m[3][2] + m1._m[3][3] * m2._m[3][3];

	return result;
}

/**
* @brief Create a orthograpic projection matrix
*
* @param left	Left boundary
* @param right	Right boundary
* @param top	Top boundary
* @param bottom	Bottom boundary
* @param znear	Z near boundary
* @param zfar	Z far boundary
*
* @result return ortho matrix
*/
template<typename T>
Matrix4<T> Ortho(T left, T right, T top, T bottom, T znear, T zfar)
{
	Matrix4<T> result(true);

	T rightMinusLeftInv = 1 / (right - left);
	T topMinusBottomInv = 1 / (top - bottom);
	T farMinusNearInv = 1 / (zfar - znear);

	result._m[0][0] = 2 * rightMinusLeftInv;
	result._m[1][1] = 2 * topMinusBottomInv;
	result._m[2][2] = -2 * farMinusNearInv;
	result._m[0][3] = -(right + left) * rightMinusLeftInv;
	result._m[1][3] = -(top + bottom) * topMinusBottomInv;
	result._m[2][3] = -(zfar + znear) * farMinusNearInv;

	return result;
}

}

/** @}*/

