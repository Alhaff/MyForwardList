#pragma once
#include <memory>
template <class T>
class forward_list;

template <class T>
class forward_list_iterator;

template <class T>
class Node
{
	friend class forward_list_iterator<T>;
	friend class forward_list<T>;
private:
	T _value;
	std::unique_ptr<Node<T>> _next;
	Node(const T& value, Node<T>* next = nullptr)
		: _value(value),
		_next(next)
	{}

	Node(T&& value, std::unique_ptr<Node<T>>&& next)
		: _value(std::move(value)), _next(std::move(next))
	{
		next = nullptr;
	}
};