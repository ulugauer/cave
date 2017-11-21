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

#include <memory>
#include <cassert>

namespace cave
{
/// Implements a subset of std::vector
template <typename T>
class caveVector
{
public:
	/// A type that counts the number of elements in a vector
	typedef size_t size_type;
	/// A type that provides a random-access iterator that can read or modify any element in a vector
	typedef T* iterator;

	/** default constructor */
	caveVector(std::shared_ptr<AllocatorBase> allocator) 
		: _allocator(allocator)
		, _data(0), _size(0), _capacity(0) {}

	/** 
	* @brief fill constructor
	* 
	* param[in] size	Vector size
	*/
	caveVector(std::shared_ptr<AllocatorBase> allocator, size_type size) 
		: _allocator(allocator)
		, _data(0), _size(0), _capacity(0)
	{
		Reserve(size);

		// initialize new items
		for (size_type i = 0; i < size; i++)
			_data[i] = T();

		_size = size;
	}

	/** 
	* @brief copy constructor
	*
	* param[in] rhs	Source Vector
	*/
	caveVector(const caveVector<T>& rhs) 
		: _data(0), _size(0), _capacity(0)
	{
		_allocator = rhs._allocator;

		Reserve(rhs.size());
		for (size_t i = 0; i < rhs.size(); i++)
			_data[i] = rhs[i];

		_size = rhs.size();
	}

	/** @brief Destructor */
	~caveVector()
	{
		Clear();
	}

	/** @brief Copy assignment */
	inline caveVector& operator=(const caveVector& rhs)
	{
		Clear();
		Reserve(rhs.size());
		for (size_t i = 0; i < rhs.size(); i++)
			_data[i] = rhs[i];

		_size = rhs.Size();

		return *this;
	}

	/**
	* @brief Adds an element to the end of the vector
	*
	* @param[in] value	The element added to the end of the vector
	*
	*/
	inline void Push(const T& value)
	{
		CheckAlloc();
		_data[_size++] = value;
	}

	/** @brief Deletes the element at the end of the vector */
	inline void Pop()
	{
		if (_size > 0)
			_size--;
	}


	/**
	* @brief Returns a reference to the vector element at a specified position
	*
	* @param[in] index he position of the vector element
	*
	* @return If the position specified is greater than the size of the container,
	*         the result is undefined
	*/
	inline T& operator[](size_type index)
	{
		// array bound check, special case where _size is 0, this is allowed when index is 0
		assert((index < _size) || ((_size == 0) && (index == 0)));
		return _data[index];
	}

	/**
	* @brief Returns a reference to the vector element at a specified position
	*
	* @param[in] index	The position of the vector element
	*
	* @return If the position specified is greater than the size of the container,
	*         the result is undefined
	*/
	inline const T& operator[](size_type index) const
	{
		// array bound check, special case where _size is 0, this is allowed when index is 0
		assert((index < _size) || ((_size == 0) && (index == 0)));
		return _data[index];
	}


	/** @brief Erases the elements of the vector */
	void Clear()
	{
		// prevents double deletion
		if (_capacity != 0)
		{
			DeallocateArray<T>(*_allocator, _data);
			_data = 0;
			_capacity = 0;
			_size = 0;
		}

	}

	/**
	* @brief Return the number of elements in the vector
	*
	* @return the number of elements in the vector
	*/
	inline size_type Size() const
	{
		return _size;
	}

	/// @brief Tests if the vector is empty
	///
	/// @return true if the vector is empty; false if the vector is not empty
	inline bool Empty() const
	{
		return (_size == 0);
	}



	/**
	* @brief Returns a pointer to the first data element
	*
	* @return A pointer to the allocated data
	*/
	T* Data()
	{
		return &_data[0];
	}

	/**
	* @brief Returns a reference to the first element in a vector.
	*
	* @return A reference to the first element in the vector object. If the vector is empty, the return is undefined.
	*/
	T& Front()
	{
		return _data[0];
	}


	/**
	/// @brief Erases a vector and copies the specified elements to the empty vector.
	///
	/// @param[in] count	The number of copies of an element being inserted into the vector.
	/// @param[in] val		The value of the element being inserted into the vector.
	*/
	void Assign(size_type count, const T& val)
	{
		Clear();
		Reserve(count);

		for (size_t i = 0; i < count; i++)
			_data[i] = val;
	}


	/**
	* @brief Reserves a minimum length of storage for a vector object, allocating space if necessary
	*
	* @param[in] minSize	The minimum length of storage to be allocated for the vector
	*/
	void Reserve(size_type minSize)
	{
		if (minSize > _capacity)
		{
			// For sizeof(T) == 1 (i.e. char, unsigned char), round up to 16 byte increments
			const size_t roundedSize = sizeof(T) == 1 ? ((minSize + 15) & ~15) : minSize;
			T* tmp = AllocateArray<T>(*_allocator, roundedSize);

			if (_data != 0)
			{
				for (size_t i = 0; i < _size; i++)
					tmp[i] = _data[i];

				DeallocateArray<T>(*_allocator, _data);
			}

			_capacity = roundedSize;
			_data = tmp;
		}
	}

	/**
	* @brief Specifies a new size for a vector
	*
	* @param[in] newSize	The new size of the vector
	*
	*/
	void Resize(size_type newSize)
	{
		if (newSize != _size)
		{
			size_type itemsToCopy;
			size_type oldSize = _size;
			T* tmp = AllocateArray<T>(*_allocator, newSize);

			_capacity = _size = newSize;

			itemsToCopy = oldSize;
			if (newSize < oldSize)
				itemsToCopy = newSize;

			for (size_t i = 0; i < itemsToCopy; i++)
				tmp[i] = _data[i];

			if (_data)
				DeallocateArray<T>(*_allocator, _data);

			_data = tmp;
		}
	}


private:

	inline void CheckAlloc()
	{
		if ((_size + 1) > _capacity)
		{
			size_type newSize;

			if (_capacity < 16)
			{
				newSize = _capacity + 1;
			}
			else
			{
				static const size_type maxVectorIncrement = 512;
				if (_capacity < maxVectorIncrement)
					newSize = _capacity * 2;
				else
					newSize = _capacity + maxVectorIncrement;
			}

			Reserve(newSize);
		}
	}


	std::shared_ptr<AllocatorBase> _allocator;	///< Pointer to global allocator
	T*     _data;			///< Pointer to data
	size_type _size;		///< Current used size
	size_type _capacity;	///< Actual allocated size

};

}

/** @}*/

