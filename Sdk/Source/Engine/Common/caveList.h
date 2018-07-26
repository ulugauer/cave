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
/// Implements a subset of std::list
template <typename T>
class CaveList
{
private:
	struct node;        ///< internal node struct

public:
	/** The list iterator class */
	class Iterator {
	public:
		/** @brief Get next element in list prefix operator */
		inline void operator++() { _value = _value->_next; }
		/** @brief Get next element in list postfix operator */
		inline void operator++(int) { _value = _value->_next; }
		/** 
		* @brief compare to list elements
		*
		* @param[in] a The element we compare with
		*/
		inline bool operator!=(const Iterator& a) const { return _value != a._value; }
		/** 
		* @brief compare to list elements
		*
		* @param[in] a the element we compare with
		*/
		inline bool operator==(const Iterator& a) const { return _value == a._value; }
		/** 
		* @brief get the value from the element container
		*
		* @return element data
		*/
		inline T& operator*() const { return _value->_data; }
		/** 
		* @brief get the reference from the element container
		*
		* @return element reference
		*/
		inline T* operator->() const { return &_value->_data; }
		/** 
		* @brief get the next element from the list
		*
		* @return next element
		*/
		inline Iterator next() const { Iterator tmp; tmp._value = _value->_next; return tmp; }
	private:
		friend class CaveList;       ///< full access for the container class
		node* _value;               ///< internal list node
	};

	/** default constructor */
	CaveList(std::shared_ptr<AllocatorBase> allocator)
		: _allocator(allocator)
		, _head(nullptr), _tail(nullptr) 
	{
		
	}

	/** @brief Destructor */
	~CaveList()
	{
		Clear();
	}

	/** @brief Clear list */
	void Clear()
	{
		while (_head != nullptr)
		{
			node* tmp = _head;
			_head = _head->_next;
			DeallocateDelete(*_allocator, *tmp);
		}
		_head = nullptr;
		_tail = nullptr;
	}

	/**
	* @brief Add element to the end of the list
	*
	* @param[in] t	Pointer to list node
	*
	* @return true on success
	*/
	inline bool PushBack(const T& t)
	{
		node* tmp = AllocateObject<node>(*_allocator, t);

		if (!tmp)
			return false;

		tmp->_next = nullptr;

		if (_head == nullptr)
		{
			tmp->_prev = nullptr;
			_head = tmp;
			_tail = tmp;
		}
		else
		{
			assert(_tail != nullptr);
			_tail->_next = tmp;
			tmp->_prev = _tail;
			_tail = tmp;
		}

		return true;
	}

	/**
	* @brief Add element to the beginning of the list
	*
	* @param[in] t	Pointer to list node
	*
	* @return true on success
	*/
	inline bool PushFront(const T& t)
	{
		node* tmp = AllocateObject<node>(*_allocator, t);

		if (!tmp)
			return false;

		tmp->_prev = nullptr;

		if (_head == nullptr)
		{
			tmp->_next = nullptr;
			_head = tmp;
			_tail = tmp;
		}
		else
		{
			assert(_tail != nullptr);
			tmp->_next = _head;
			_head->_prev = tmp;
			_head = tmp;
		}

		return true;
	}

	/**
	* @brief Remove element from list
	*
	* @param[in] iter	Elemet to remove
	*
	*/
	void Erase(Iterator &iter)
	{
		node* tmp = iter._value;

		iter++;

		Unqueue(tmp);

		DeallocateDelete(*_allocator, *tmp);
	}

	/**
	* @brief Remove element from list
	*
	* @return List element count
	*/
	inline int32_t Size()
	{
		int count = 0;
		node *tmp = _head;
		if (_head == _tail && _head != nullptr)
			return 1;

		while (tmp != nullptr)
		{
			tmp = tmp->_next;
			count++;
		}
		return count;
	}

	/**
	* @brief Returns a random-access iterator to the first element in the container
	*
	* @return A random-access iterator addressing the first element in the list
	*/
	inline Iterator begin() const { Iterator tmp; tmp._value = _head; return tmp; }

	/** 
	* @brief Returns a random-access iterator to the last element in the container
	*
	* @return A random-access iterator addressing the last element in the list
	*/
	inline Iterator tail()  const { Iterator tmp; tmp._value = _tail; return tmp; }

	/** 
	* @brief Returns a end iterator
	*
	* @return end iterator
	*/
	static inline Iterator end() { Iterator tmp; tmp._value = nullptr;  return tmp; }

	/** 
	* @brief Returns a null iterator
	*
	* @return null iterator
	*/
	static inline Iterator null() { Iterator tmp; tmp._value = nullptr;  return tmp; }


private:
	/**
	* Internal list node
	*/
	struct node
	{
		/** @brief constructor */
		node(const T& t) { _data = t; }

		T _data;	///< Pointer to data

		node* _next;	///< Pointer to next element
		node* _prev;	///< Pointer to previous element
	};

	/** 
	* @brief Remove element from list
	*
	* @param[in] node	Pointer to list node
	*/
	inline void Unqueue(node *node)
	{
		if (node->_next == nullptr)
		{
			// if next == NULL, then we must be at the tail
			assert(node == _tail);
			_tail = _tail->_prev;
		}
		else
		{
			node->_next->_prev = node->_prev;
		}

		if (node->_prev == nullptr)
		{
			// if prev == NULL, then we must be at the head
			assert(node == _head);
			_head = _head->_next;
			if (_head == nullptr)
				_tail = nullptr;
		}
		else
		{
			node->_prev->_next = node->_next;
		}
	}

	/**
	* @brief Add element to list
	*
	* @param[in] where	Iterator to insertion place
	* @param[in] what	Pointer to node
	*/
	inline void Queue(Iterator where, node *what)
	{
		node* tmp = where._value;

		if (tmp == nullptr)
		{
			if (_tail)
			{
				// insert as tail
				what->_prev = _tail;
				what->_next = nullptr;
				_tail->_next = what;
				_tail = what;
			}
			else
			{
				// empty list
				assert(_head == nullptr);
				_head = _tail = what;
				what->_next = what->_prev = nullptr;
			}
		}
		else
		{
			what->_prev = tmp->_prev;
			what->_next = tmp;
			if (tmp->_prev == nullptr)
			{
				// if prev == NULL, then we must be at the head
				ctkAssert(tmp == _head);
				_head = what;
				assert(_tail != nullptr);
			}
			else
			{
				tmp->_prev->_next = what;
			}
			tmp->_prev = what;
		}
	}

	
private:
	std::shared_ptr<AllocatorBase> _allocator;	///< Pointer to global allocator
	node* _head;		///< List start pointer
	node* _tail;		///< List end pointer

};

}

/** @}*/


