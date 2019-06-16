#pragma once
#include <stdexcept>
#include "Item.hpp"

template <class T>
class Stack 
{
	private:
		Item<T>* topItem;
		int size;
	public:
		Stack();
		~Stack();

		// DATA MANIPULATION FUNCTIONS
		void push(T value);
		T pop();
			// Operators
			void operator<< (T value);
			T operator--();

		// CONSULTATION FUNCTIONS
		T top();
		int count();

		// UTIL FUNCTIONS
		bool isEmpty();
};
