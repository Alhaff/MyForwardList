#pragma once
#include <memory>
#include <functional>
#include"node.h"
#include "forward_list_iterator.h"

template <class T>
using SplinPredicate = std::function<bool(const T& value)>;

template <class T>
class forward_list
{
private:
	std::unique_ptr<Node<T>> before_head;
	size_t _size;
public:
	//Member functions
	forward_list();
	forward_list(size_t size, const T& value);
	forward_list(size_t size);
	forward_list(const forward_list<T>& other);
	forward_list(forward_list<T>&& other);
	forward_list(std::initializer_list<T> init);
	forward_list<T>& operator=(const forward_list<T>& other);
	forward_list<T>& operator=(forward_list<T>&& other);
	forward_list<T>& operator=(std::initializer_list<T> init);
	void assign(size_t count, const T& value);
	void assign(std::initializer_list<T> ilist);
	template< class InputIt >
	void assign(InputIt first, InputIt last);
	~forward_list();
public:
	//Element Access
	T& front();
	const T& front() const;
public:
	//Iterators
	typedef forward_list_iterator<T> iterator;
	typedef const forward_list_iterator<T> const_iterator;
	iterator begin();
	iterator end();
	iterator before_begin();
	const_iterator cbegin() const;
	const_iterator cend() const;
	const_iterator cbefore_begin() const;
public:
	//Capacity
	bool empty();
public:
	//Modifiers	
	void push_front(const T& value);
	void push_front(T&& value);
	void pop_front();
	size_t size() const;
	void resize(size_t count, const T& value = T());
	void swap(forward_list& other) noexcept;
	void clear();
	iterator insert_after(const_iterator pos, const T& value);
	iterator insert_after(const_iterator pos, T&& value);
	iterator insert_after(const_iterator pos, size_t count, const T& value);
	template< class InputIt >
	iterator insert_after(const_iterator pos, InputIt first, InputIt last);
	iterator insert_after(const_iterator pos, std::initializer_list<T> ilist);
	iterator erase_after(const_iterator pos);
	iterator erase_after(const_iterator first, const_iterator last);
public:
	//Special functions
	forward_list<T> splin_when(SplinPredicate<T> condition);
};




//Member functions
template<class T>
inline forward_list<T>::forward_list()
	: before_head(new Node<T>(T())),
	_size(0) {}

template<class T>
inline forward_list<T>::forward_list(size_t size, const T& value)
	: before_head(new Node<T>(value))
{
	while (_size != size)
	{
		push_front(value);
	}
}

template<class T>
inline forward_list<T>::forward_list(size_t size)
	: before_head(new Node<T>(T()))
{
	while (_size != size)
	{
		push_front(T());
	}
}

template<class T>
inline forward_list<T>::forward_list(const forward_list<T>& other)
	:  before_head(new Node<T>(other.before_head->_value)), _size(0)
{
	push_front(other.cbegin()._node->_value);
	Node<T>* curr = begin()._node;
	Node<T>* other_curr = other.cbegin()._node->_next.get();
	while (_size != other._size)
	{
		curr->_next.reset(new Node<T>(other_curr->_value));
		curr = curr->_next.get();
		other_curr = other_curr->_next.get();
		++_size;
	}
}

template<class T>
inline forward_list<T>::forward_list(forward_list<T>&& other)
	: before_head(new Node<T>(std::move(other.before_head->_value))), _size(other._size)
{
	before_head->_next = std::move(other.before_head->_next);
	other._size = 0;
	other.before_head->_next = nullptr;
}

template<class T>
inline forward_list<T>::forward_list(std::initializer_list<T> init)
	: before_head(new Node<T>(T())),
	_size(0)
{
	if (std::begin(init) != std::end(init))
	{
		auto elem = std::begin(init);
		push_front(*elem);
		Node<T>* curr = begin()._node;
		for (++elem; elem != std::end(init); ++elem)
		{
			curr->_next.reset(new Node<T>(*elem));
			curr = curr->_next.get();
			++_size;
		}
	}
}

template<class T>
inline forward_list<T>& forward_list<T>::operator=(const forward_list<T>& other)
{
	if (this != &other)
	{
		resize(other._size);
		auto our_it = begin();
		auto other_it = other.cbegin();
		for(size_t i = 0; i < _size && i < other._size; ++i)
		{
			*our_it = *other_it;
			++our_it;
			++other_it;
		}
	}
	return *this;
}

template<class T>
inline forward_list<T>& forward_list<T>::operator=(forward_list<T>&& other)
{
	if (this != &other)
	{
		before_head->_next = std::move(other.before_head->_next);
		_size = std::move(other._size);
		other._size = 0;
		other.before_head->_next = nullptr;
	}
	return *this;
}

template<class T>
inline forward_list<T>& forward_list<T>::operator=(std::initializer_list<T> init)
{
	assign(init);
	return *this;
}

template <class T>
inline void forward_list<T>::assign(size_t count, const T& value)
{
	auto it = before_begin();
	for (size_t i = 0; i < _size && i < count; i++)
	{
		++it;
		*it = value;
	}
	if (count < _size)
	{
		insert_after(it, _size - count + 1, value);
	}
	else if (_size > count)
	{
		erase_after(it, end());
	}
}

template <class T>
inline void forward_list<T>::assign(std::initializer_list<T> ilist)
{
	assign(std::begin(ilist), std::end(ilist));
}

template<class T>
template<class InputIt>
inline void forward_list<T>::assign(InputIt first, InputIt last)
{
	if (first != nullptr)
	{
		auto init_it = first;
		auto it = before_begin();
		for (size_t i = 0; i < _size && init_it != last; i++)
		{
			++it;
			*it = *init_it;
			++init_it;
		}
		if (init_it != last)
		{
			insert_after(it, init_it, last);
		}
		else if (it != end())
		{
			erase_after(it, end());
		}
	}
	else 
	{
		clear();
	}

}

template<class T>
inline forward_list<T>::~forward_list()
{
	clear();
}

//Element Access
template<class T>
inline T& forward_list<T>::front()
{
	return *begin();
}

template<class T>
inline const T& forward_list<T>::front() const
{
	return *cbegin();
}

//iterators
template<class T>
inline forward_list_iterator<T> forward_list<T>::begin()
{
	return iterator(before_head->_next.get());
}

template<class T>
inline forward_list_iterator<T> forward_list<T>::end()
{
	return iterator(nullptr);
}

template<class T>
inline forward_list_iterator<T> forward_list<T>::before_begin()
{
	return iterator(before_head.get());
}

template<class T>
inline const forward_list_iterator<T> forward_list<T>::cbegin() const
{
	return const_iterator(before_head->_next.get());
}

template<class T>
inline const forward_list_iterator<T>  forward_list<T>::cend() const
{
	return const_iterator(nullptr);
}

template<class T>
inline const forward_list_iterator<T>  forward_list<T>::cbefore_begin() const
{
	return const_iterator(before_head.get());
}
//Capacity
template<class T>
inline bool  forward_list<T>::empty()
{
	return (_size == 0);
}
//Modifiers	
template<class T>
inline void forward_list<T>::push_front(const T& value)
{
	Node<T>* temp_head = before_head->_next.release();
	before_head->_next.reset(new Node<T>(value, temp_head));
	++_size;
}

template<class T>
inline void forward_list<T>::push_front(T&& value)
{
	before_head->_next.reset(new Node<T>(std::move(value), std::move(before_head->_next)));
	++_size;
}

template<class T>
inline void forward_list<T>::pop_front()
{
	before_head->_next = std::move(before_head->_next->_next);
	--_size;
}

template<class T>
inline size_t forward_list<T>::size() const
{
	return _size;
}

template<class T>
inline void forward_list<T>::resize(size_t count, const T& value)
{
	if (_size > count)
	{
		iterator start = begin();
		for (size_t i = 0; i < _size; i++)
		{
			if (i == count)
			{
				erase_after(start, end());
				break;
			}
			++start;
		}
	}
	else if (_size < count)
	{
		iterator start = before_begin();
		iterator _begin = begin();
		while (_begin != end())
		{
			++_begin;
			++start;
		}
		insert_after(start, count - _size + 1, value);
	}
}

template<class T>
inline void forward_list<T>::swap(forward_list& other) noexcept
{
	before_head.swap(other.before_head);
	std::swap(_size, other._size);
}

template<class T>
inline void forward_list<T>::clear()
{
	while (_size)
	{
		pop_front();
	}
}

template<class T>
inline forward_list_iterator<T> forward_list<T>::insert_after(const_iterator pos, const T& value)
{
	Node<T>* temp = pos._node->_next.release();
	auto el = std::move(std::unique_ptr<Node<T>>(new Node<T>(value, temp)));
	pos._node->_next = std::move(el);
	++_size;
	return iterator(pos._node->_next.get());
}

template<class T>
inline forward_list_iterator<T> forward_list<T>::insert_after(const_iterator pos, T&& value)
{
	pos._node->_next.reset(new Node<T>(std::move(value), std::move(pos._node->_next)));
	++_size;
	return iterator(pos._node->_next.get());
}

template<class T>
inline forward_list_iterator<T> forward_list<T>::insert_after(const_iterator pos, size_t count, const T& value)
{
	iterator temp_it = pos;
	while (--count)
	{
		temp_it = insert_after(temp_it, value);
	}
	return temp_it;
}

template<class T>
template<class InputIt>
inline forward_list_iterator<T> forward_list<T>::insert_after(const_iterator pos, InputIt first, InputIt last)
{
	iterator temp_it = pos;
	for (auto it = first; it != last; ++it)
	{
		temp_it = insert_after(temp_it, *it);
	}
	return temp_it;
}

template<class T>
inline forward_list_iterator<T> forward_list<T>::insert_after(const_iterator pos, std::initializer_list<T> ilist)
{
	return insert_after(pos, std::begin(ilist), std::end(ilist));
}

template<class T>
inline forward_list_iterator<T> forward_list<T>::erase_after(const_iterator pos)
{
	pos._node->_next = std::move(pos._node->_next->_next);
	--_size;
	return iterator(pos._node->_next.get());
}

template<class T>
inline forward_list_iterator<T> forward_list<T>::erase_after(const_iterator first, const_iterator last)
{
	while (iterator(first._node->_next.get()) != last)
	{
		erase_after(first);
	}
	return last;
}

//Special functions
template<class T>
inline forward_list<T> forward_list<T>::splin_when(SplinPredicate<T> condition)
{
	size_t counter = 0;
	for (auto it = before_begin(); it._node->_next != nullptr; it++)
	{
		forward_list<T> temp;
		if (condition(it._node->_next->_value))
		{
			temp._size = _size - counter;
			_size = counter;
			temp.before_head->_next = (std::move(it._node->_next));
			return std::move((temp));
		}
		++counter;
	}
	return std::move((forward_list<T>()));
}