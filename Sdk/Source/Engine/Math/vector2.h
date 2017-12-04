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

/// @file vector2.h
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
* @brief Vector 2 implementation
*/
template<typename T>
class CAVE_INTERFACE Vector2
{
public:
	T _x;	///< x componente
	T _y;	///< y componente

	/** default constructor */
	Vector2<T>() {};

	/** constructor
	* param[in] x	x value
	*/
	Vector2<T>(T x)
		: _x(x), _y(x)
	{}

	/** constructor
	* param[in] x	x value
	* param[in] y	y value
	*/
	Vector2<T>(T x, T y)
		: _x(x), _y(y)
	{}

	/** destructor */
	~Vector2<T>() {}

	/** copy ctor */
	Vector2<T>(const Vector2<T>& v)
		: _x(v._x), _y(v._y)
	{}

	/** assignemnt operator */
	Vector2<T>& operator=(const Vector2<T>& v)
	{
		_x = v._x; _y = v._y;
		return *this;
	}

	/** operator + */
	Vector2<T> operator+(const Vector2<T>& v) const
	{
		return Vector2<T>(_x + v._x, _y + v._y);
	}
	/** operator - */
	Vector2<T> operator-(const Vector2<T>& v) const
	{
		return Vector2<T>(_x - v._x, _y - v._y);
	}
	/** operator scalar mul */
	Vector2<T> operator*(T s) const
	{
		return Vector2<T>(_x*s, _y*s);
	}
	/** operator scalar div */
	Vector2<T> operator/(T s) const
	{
		T inverseS = 1.0 / s;
		return Vector2<T>(_x*inverseS, _y*inverseS);
	}

	/** inline vector normalization */
	void Normalize()
	{
		T magSquared = _x*_x + _y*_y;
		if (magSquared > 0)
		{
			T inverseMag = 1.0 / std::sqrt(magSquared);
			_x *= inverseMag;
			_y *= inverseMag;
		}
	}

};

typedef Vector2<float> Vector2f;		///< type specialization
typedef Vector2<int32_t> Vector2i;	///< type specialization
typedef Vector2<uint32_t> Vector2ui;	///< type specialization


}

/** @}*/

