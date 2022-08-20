#pragma once
#include <memory>
#include "node.h"
template <class T>
class forward_list;

template <class T>
class forward_list_iterator;

template <class T>
class forward_list_iterator : std::iterator<std::forward_iterator_tag, T>
{
	friend class forward_list<T>;

private:
	Node<T>* _node;
	forward_list_iterator(Node<T>* node)
		: _node(node) {}
public:
	forward_list_iterator(const forward_list_iterator& other)
		: _node(other._node) {}

	forward_list_iterator& operator=(const forward_list_iterator& other)
	{
		if (this != &other)
		{
			_node = other._node;
		}
		return *this;
	}

	bool operator !=(const forward_list_iterator& other) const
	{
		return _node != other._node;
	}

	bool operator ==(const forward_list_iterator& other) const
	{
		return _node == other._node;
	}

	typename forward_list_iterator::reference operator*() const
	{
		return (_node->_value);
	}

	forward_list_iterator& operator++()
	{
		_node = (_node->_next.get());
		return *this;
	}

	forward_list_iterator& operator++(int)
	{
		forward_list_iterator temp(_node);
		_node = _node->_next.get();
		return temp;
	}

	const forward_list_iterator& operator++() const
	{
		_node = (_node->_next.get());
		return *this;
	}

	const forward_list_iterator& operator++(int) const
	{
		forward_list_iterator temp(_node);
		_node = _node->_next.get();
		return temp;
	}

	operator forward_list_iterator<const T>()
	{
		return forward_list_iterator<T>(_node);
	}
};