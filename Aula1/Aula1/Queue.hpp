#pragma once
#include <stdexcept>
#include "Item.hpp"

template <class T>
class Queue
{
	private:
		Item<T>* firstItem;
		Item<T>* lastItem;
		int size;

	public:
		Queue();
		~Queue();

		// DATA MANIPULATION FUNCTIONS
		void push(T value);
		T pop();
			// Operators
			void operator<< (T value);
			T operator--();

		// CONSULTATION FUNCTIONS
		T first();
		int count();

		// UTIL FUNCTIONS
		bool isEmpty();
};
