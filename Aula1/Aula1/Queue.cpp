#pragma once
#include "Queue.hpp"

template<class T>
Queue<T>::Queue()
{
	firstItem = nullptr;
	lastItem = nullptr;
	size = 0;
}

template<class T>
Queue<T>::~Queue() {}

template<class T>
void Queue<T>::push(T value)
{
	auto* newItem = new Item<T>(); // Creates a new Item
	newItem->setValue(value);      // Setting the value
	
	if(isEmpty())
		firstItem = newItem;             // Handling first iteration
	else
		lastItem->setPrevious(newItem);  // Sets the next in queue of the current last in queue

	lastItem = newItem;
	size++;
}

template<class T>
T Queue<T>::pop()
{
	if (isEmpty())
		throw std::out_of_range("Queue is Empty!"); // Cannot return if the Queue is empty

	auto* oldItem = firstItem;             // Saves the pointer of the item to be removed
	T oldItemValue = oldItem->getValue();  // Saves the value of the oldItem
	
	if (firstItem == lastItem)             // If the queue is emptying (size 1)
		lastItem = nullptr;                // Sets the lastItem as nullptr

	firstItem = oldItem->getPrevious();    // Sets the oldItem's next in queue as the current item
	delete oldItem;                        // Deletes the oldItem from memory
	size--;                                // Decreases the Queue size

	return oldItemValue;                   // Returns the oldItem's value
}

template<class T>
void Queue<T>::operator<<(T value)
{
	push(value);
}

template<class T>
T Queue<T>::operator--()
{
	return pop();
}

template<class T>
T Queue<T>::first()
{
	if (isEmpty())
		throw std::out_of_range("Queue is Empty!"); // Cannot return if the Queue is empty

	return firstItem->getValue();
}

template<class T>
int Queue<T>::count()
{
	return size;
}

template<class T>
bool Queue<T>::isEmpty()
{
	return (lastItem == nullptr);
}

template class Queue<int>;
template class Queue<float>;
template class Queue<double>;
template class Queue<char>;
template class Queue<bool>;
template class Queue<std::string>;
