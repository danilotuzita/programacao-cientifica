#pragma once

#include <string>;

template <class T>
class Item
{
	private:
		T value;
		Item* previous;

	public:
		Item();
		~Item();

		T getValue() const;
		Item *getPrevious() const;

		void setValue(T value);
		void setPrevious(Item *previous);
};

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
