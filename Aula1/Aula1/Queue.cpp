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

}

template<class T>
T Queue<T>::pop()
{
}


template class Queue<int>;
template class Queue<float>;
template class Queue<double>;
template class Queue<char>;
template class Queue<bool>;
template class Queue<std::string>;
