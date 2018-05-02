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

/** \addtogroup engine
*  @{
*
*/

#include "engineDefines.h"
#include "Memory/allocatorBase.h"
#include "caveVector.h"

#include <memory>
#include <cassert>


namespace cave
{
/// Implements a subset of std::string
class caveString
{
public:
	/// A type that provides a random-access iterator that can read or modify any element in a vector
	typedef char* iterator;
	/// An unsigned integer type that can represent the number of elements and indices in a string.
	typedef caveVector<char>::size_type size_type;
	/// An unsigned integral value initialized to –1 that indicates either "not found" or "all remaining characters"
	/// when a search function fails.
	static const size_type npos = (size_type)-1;


	/**
	* @brief default constructor
	*
	* @param[in] allocator	Pointer to memory allocator
	*/
	caveString(std::shared_ptr<AllocatorBase> allocator)
		: _str(allocator)
		{}

	/**
	* @brief copy constructor
	*
	* @param[in] allocator	Pointer to memory allocator
	* @param[in] rhs		The original string object
	*/
	caveString(std::shared_ptr<AllocatorBase> allocator, const caveString& rhs) :
		_str(allocator)
	{
		Append(rhs.c_str());
	}

	/**
	* @brief constructor
	*
	* @param[in] allocator	Pointer to memory allocator
	* @param[in] str		Pointer to string
	*/
	caveString(std::shared_ptr<AllocatorBase> allocator, const char* str)
		: _str(allocator)
	{
		if (str)
			Append(str);
	}

	/** destructor */
	~caveString() {}

	/**
	* @brief assignment operator
	*
	* @param[in] str	Right hand side string object
	*/
	caveString& operator=(const caveString& str)
	{
		_str.Clear();
		Append(str.c_str());

		return *this;
	}

	/**
	* @brief assignment operator
	*
	* @param[in] str	Pointer to char string
	*/
	caveString& operator=(const char* str)
	{
		_str.Clear();
		Append(str);

		return *this;
	}

	/**
	* @brief assignment operator
	*
	* @param[in] c		Character to find
	* @param[in] offset	Offset to start from
	*
	* @return pos if found or npos
	*/
	size_type FindFirstOf(char c, size_type offset = 0)
	{
		iterator s;
		size_type idx = 0;

		for (s = _str.Begin() + offset; s != _str.End(); s++, ++idx)
		{
			if (*s == c) break;
		}

		// found ?
		if (idx == _str.Size())
			return npos;

		return idx + offset;
	}


	/** 
	* @brief Return the length of the string (without the NULL terminating character)
	*
	* @return the length of the string (without the NULL terminating character)
	*/
	size_type Length() const { return _str.Size() == 0 ? 0 : _str.Size() - 1; }

	/**
	* @brief Return the length of the string (without the NULL terminating character)
	*
	* @return the length of the string (without the NULL terminating character)
	*/
	size_type Size() const { return _str.Size() == 0 ? 0 : _str.Size() - 1; }

	/**
	* Adds characters to the end of a string.
	*
	* @param[in] str The string whose characters are to be appended.
	*/
	void Append(const char* str)
	{
		if (str != 0)
		{
			size_type oldSize = Length();
			size_type newSize = oldSize + strlen(str) + 1;
			_str.Resize(newSize);

			// copy the string (including the \0)
			for (size_type i = oldSize; i < newSize; i++)
			{
				_str[i] = *str++;
			}
		}
	}

	/**
	* Adds characters to the end of a string.
	*
	* @param[in] str	The string whose characters are to be appended.
	* @param[in] size	The number of characters to be appended, at most, from the source string.

	*/
	void Append(const char *str, size_type size)
	{
		if (str != 0)
		{
			size_type oldSize = Length();
			size_type newSize = oldSize + size;
			_str.Resize(newSize + 1);

			// copy the string
			for (size_type i = oldSize; i < newSize; i++)
			{
				_str[i] = *str++;
			}
			_str[newSize] = 0;
		}
	}

	/**
	* @brief Erases all elements of a string.
	*/
	void clear()
	{
		_str.Clear();
	}


	/**
	* Return a pointer to the first element of the string
	*
	* @return a pointer to the first element of the string
	*/
	const char* c_str() const { return (_str.Size() == 0) ? 0 : &_str[0]; }

	/**
	* Return the character at the given index in the vector
	*
	* @param[in] index	Index location of string
	*
	* @return char at index location
	*/
	char& operator[](size_type index) { return _str[index]; }
	/**
	* Return the const character at the given index in the vector
	*
	* @param[in] index	Index location of string
	*
	* @return const char at index location
	*/
	const char& operator[](size_type index) const { return _str[index]; }

private:
	caveVector<char> _str;		///< The vector behind the string
};

/**
* Compare operator
*
* @param[in] a	String 1 to compare
* @param[in] b	String 2 to compare
*
* @return true if equal
*/
inline bool operator==(const caveString& a, const caveString& b)
{
	return strcmp(a.c_str(), b.c_str()) == 0;
}


}

/** @}*/
