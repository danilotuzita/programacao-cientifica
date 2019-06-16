#pragma once
#include "Stack.hpp"

template < class T >
Stack<T>::Stack()
{
	topItem = nullptr; // Initializing the Stack
	size = 0;          // Setting its size to 0
}

template < class T >
Stack<T>::~Stack() {}

template < class T >
void Stack<T>::push(T value)
{
	auto* newItem = new Item<T>();     // Creates a new Item
	newItem->setValue(value);          // Setting the value
	if (!isEmpty())                    // Check stack is not empty
		newItem->setPrevious(topItem); // Sets the current topItem as the previous

	topItem = newItem;                 // Sets the newItem as the topItem of the Stack
	size++;                            // Increases the Stack size
}

template < class T >
T Stack<T>::pop()
{
	if (isEmpty())
		throw std::out_of_range("Stack is Empty!"); // Cannot return if the Stack is empty

	auto* oldItem = topItem;              // Saves the pointer of the item to be removed
	T oldItemValue = oldItem->getValue(); // Saves the value of the oldItem
	topItem = oldItem->getPrevious();     // Sets the oldItem's bottom item as the current item
	delete oldItem;                       // Deletes the oldItem from memory
	size--;                               // Decreases the Stack size

	return oldItemValue;                  // Returns the oldItem's value
}

template<class T>
void Stack<T>::operator<<(T value)
{
	push(value);
}

template<class T>
T Stack<T>::operator--()
{
	return pop();
}

template < class T >
bool Stack<T>::isEmpty()
{
	return (topItem == nullptr);
}

template < class T >
T Stack<T>::top()
{
	if (isEmpty())
		throw std::out_of_range("Stack is Empty!"); // Cannot return if the Stack is empty

	return topItem->getValue();
}

template < class T >
int Stack<T>::count()
{
	return size;
}


template class Stack<int>;
template class Stack<float>;
template class Stack<double>;
template class Stack<char>;
template class Stack<bool>;
template class Stack<std::string>;
