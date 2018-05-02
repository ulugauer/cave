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

/// @file vector4.h
///       Handles material assets

#include "vector3.h"

/** \addtogroup engine
*  @{
*
*/

namespace cave
{


/**
* @brief Vector 4 implementation
*/
template<typename T>
class CAVE_INTERFACE Vector4
{
public:
	T _x;	///< x componente
	T _y;	///< y componente
	T _z;	///< z componente
	T _w;	///< w componente

	/** default constructor */
	Vector4<T>() {};

	/** constructor
	* param[in] x	x value
	*/
	Vector4<T>(T x)
		: _x(x), _y(x), _z(x), _w(x)
	{}

	/** constructor
	* param[in] x	x value
	* param[in] y	y value
	* param[in] z	z value
	* param[in] w	w value
	*/
	Vector4<T>(T x, T y, T z, T w)
		: _x(x), _y(y), _z(z), _w(w)
	{}

	/** constructor
	* param[in] v	3D vector
	*/
	Vector4<T>(const Vector3<T>& v)
		: _x(v._x), _y(v._y), _z(v._z), _w(1)
	{}

	/** destructor */
	~Vector4<T>() {}

	/** copy ctor */
	Vector4<T>(const Vector4<T>& v)
		: _x(v._x), _y(v._y), _z(v._z), _w(v._w)
	{}

	/** assignemnt operator */
	Vector4<T>& operator=(const Vector4<T>& v)
	{
		_x = v._x; _y = v._y; _z = v._z; _w = v._w;
		return *this;
	}

	/** operator + */
	Vector4<T> operator+(const Vector4<T>& v) const
	{
		return Vector3<T>(_x + v._x, _y + v._y, _z + v._z, _w + v._w);
	}
	/** operator - */
	Vector4<T> operator-(const Vector4<T>& v) const
	{
		return Vector3<T>(_x - v._x, _y - v._y, _z - v._z, _w - v._w);
	}
	/** operator scalar mul */
	Vector4<T> operator*(T s) const
	{
		return Vector4<T>(_x*s, _y*s, _z*s, _w*s);
	}
	/** operator scalar div */
	Vector4<T> operator/(T s) const
	{
		T inverseS = 1.0 / s;
		return Vector4<T>(_x*inverseS, _y*inverseS, _z*inverseS, _w*inverseS);
	}

	/** inline vector normalization */
	void Normalize()
	{
		T magSquared = _x*_x + _y*_y + _z*_z + _w*_w;
		if (magSquared > 0)
		{
			T inverseMag = 1.0 / std::sqrt(magSquared);
			_x *= inverseMag;
			_y *= inverseMag;
			_z *= inverseMag;
			_w *= inverseMag;
		}
	}

};

typedef Vector4<float> Vector4f;	///< type specialization
typedef Vector4<double> Vector4d;	///< type specialization

/**
* @brief normalize a vector
*
* @param vec	vector to normalize
*
* @result return normalized vector
*/
template<typename T> CAVE_INTERFACE
inline Vector4<T> Normalize(const Vector4<T>& vec)
{
	Vector4<T> res(vec);
	res.Normalize();
	return res;
}

/**
* @brief Compute vector magniture (specialized template)
*
* @param[in] v	Pointer to vector
*/
template<typename T> CAVE_INTERFACE
inline T Magnitude(const Vector4<T>& v)
{
	return std::sqrt(v._x * v._x + v._y * v._y + v._z * v._z + v._w * v._w);
}

}

/** @}*/

