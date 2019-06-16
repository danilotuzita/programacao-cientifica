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
