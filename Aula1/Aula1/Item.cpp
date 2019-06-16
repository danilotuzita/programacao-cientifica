#pragma once
#include "Item.hpp"

template < class T >
Item<T>::Item()
{
	previous = nullptr;
}

template<class T>
Item<T>::~Item() {}

template < class T >
T Item<T>::getValue() const
{
	return value;
}
template Item<int>::Item();

template < class T >
void Item<T>::setValue(T value)
{
	Item::value = value;
}

template < class T >
Item<T>* Item<T>::getPrevious() const
{
	return previous;
}

template < class T >
void Item<T>::setPrevious(Item *previous)
{
	Item::previous = previous;
}

template class Item<int>;
template class Item<float>;
template class Item<double>;
template class Item<char>;
template class Item<bool>;
template class Item<std::string>;
