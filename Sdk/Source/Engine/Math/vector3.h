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

/// @file vector3.h
///       Handles material assets

#include <limits>
#include <cmath>

/** \addtogroup engine
*  @{
*
*/

namespace cave
{


/**
* @brief Vector 3 implementation
*/
template<typename T>
class Vector3
{
public:
	T _x;	///< x componente
	T _y;	///< y componente
	T _z;	///< z componente

	/** default constructor */
	Vector3<T>() {};

	/** constructor
	* param[in] x	x value
	*/
	Vector3<T>(T x)
		: _x(x), _y(x), _z(x)
	{}

	/** constructor
	* param[in] x	x value
	* param[in] y	y value
	* param[in] z	z value
	*/
	Vector3<T>(T x, T y, T z)
		: _x(x), _y(y), _z(z)
	{}

	/** destructor */
	~Vector3<T>() {}

	/** copy ctor */
	Vector3<T>(const Vector3<T>& v)
		: _x(v._x), _y(v._y), _z(v._z)
	{}

	/** assignemnt operator */
	Vector3<T>& operator=(const Vector3<T>& v)
	{
		_x = v._x; _y = v._y; _z = v._z;
		return *this;
	}

	/** operator + */
	Vector3<T> operator+(const Vector3<T>& v) const
	{
		return Vector3<T>(_x + v._x, _y + v._y, _z + v._z);
	}
	/** operator - */
	Vector3<T> operator-(const Vector3<T>& v) const
	{
		return Vector3<T>(_x - v._x, _y - v._y, _z - v._z);
	}
	/** operator scalar mul */
	Vector3<T> operator*(T s) const
	{
		return Vector3<T>(_x*s, _y*s, _z*s);
	}
	/** operator scalar div */
	Vector3<T> operator/(T s) const
	{
		T inverseS = 1 / s;
		return Vector3<T>(_x*inverseS, _y*inverseS, _z*inverseS);
	}

	/** inline vector normalization */
	void Normalize()
	{
		T magSquared = _x*_x + _y*_y + _z*_z;
		if (magSquared > 0)
		{
			T inverseMag = 1 / std::sqrt(magSquared);
			_x *= inverseMag;
			_y *= inverseMag;
			_z *= inverseMag;
		}
	}

};

typedef Vector3<float> Vector3f;	///< type specialization
typedef Vector3<double> Vector3d;	///< type specialization


/**
* @brief normalize a vector
*
* @param vec	vector to normalize
*
* @result return normalized vector
*/
template<typename T> 
inline Vector3<T> Normalize(const Vector3<T>& vec)
{
	Vector3<T> res(vec);
	res.Normalize();
	return res;
}

/**
* @brief cross product of a x b
*
* @param a	vector a
* @param b	vector b
*
* @result return cross product vector
*/
template<typename T> 
inline Vector3<T> CrossProduct(const Vector3<T>& a , const Vector3<T>& b)
{
	Vector3<T> res;
	res._x = a._y*b._z - a._z*b._y;
	res._y = a._z*b._x - a._x*b._z;
	res._z = a._x*b._y - a._y*b._x;
	return res;
}

/**
* @brief dot product of a x b
*
* @param a	vector a
* @param b	vector b
*
* @result return dot product value
*/
template<typename T> 
inline T DotProduct(const Vector3<T>& a, const Vector3<T>& b)
{
	return a._x*b._x + a._y*b._y + a._z*b._z;
}

/**
* @brief Compute vector magniture (specialized template)
*
* @param[in] v	Pointer to vector
*/
template<typename T> 
inline T Magnitude(const Vector3<T>& v)
{
	return std::sqrt(v._x * v._x + v._y * v._y + v._z * v._z);
}

}

/** @}*/
