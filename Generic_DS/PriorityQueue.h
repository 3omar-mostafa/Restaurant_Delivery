#ifndef __PRIORITYQUEUE_H_
#define __PRIORITYQUEUE_H_
#include "Node.h"

template <typename T>
//Comparator template function.
bool isGreaterThan(T left, T right);

template <typename T>
//Compares the references pointed to by the pointers.
bool isGreaterThan(T* left, T* right);
//Both implemented in Restaurant.cpp, to be moved to a utility functions file.

template <typename T>
class PriorityQueue
{
private:
	Node<T>* backPtr;
	Node<T>* frontPtr;
	int count;
public:
	PriorityQueue();
	PriorityQueue(const PriorityQueue& copiedPQueue);
	bool isEmpty() const;
	bool enqueue(const T& newEntry);
	bool dequeue(T& frntEntry);
	bool peekFront(T& frntEntry)  const;
	int getLength() const;	
	~PriorityQueue();
};


/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: PriorityQueue()
The constructor of the PriorityQueue class.

*/

template <typename T>
PriorityQueue<T>::PriorityQueue()
{
	backPtr = nullptr;
	frontPtr = nullptr;
	count = 0;
}
template<typename T>
PriorityQueue<T>::PriorityQueue(const PriorityQueue & copiedPQueue)
{
	frontPtr = backPtr = NULL;
	count = 0;

	Node<T> *currPtr = copiedPQueue.frontPtr;
	while (currPtr)
	{
		enqueue(currPtr->getItem());
		currPtr = currPtr->getNext();
	}
}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: isEmpty
Sees whether this PriorityQueue is empty.

Input: None.
Output: True if the PriorityQueue is empty; otherwise false.
*/
template <typename T>
bool PriorityQueue<T>::isEmpty() const
{
	if (frontPtr == nullptr)
		return true;
	else
		return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

/*Function:enqueue
Adds newEntry to the PriorityQueue
The entry is inserted sorted into the Queue

Input: newEntry .
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool PriorityQueue<T>::enqueue(const T& newEntry)
{
	Node<T>* newNodePtr = new Node<T>(newEntry);
// Insert the new node:

	// If the queue is empty
	if (isEmpty())
	{
		frontPtr = newNodePtr;
		backPtr = newNodePtr;
		count++;
		return true;
	}

	// If the new node has a higher priority than the front node
	if (isGreaterThan(newNodePtr->getItem(), frontPtr->getItem()))
	{
		newNodePtr->setNext(frontPtr);
		frontPtr = newNodePtr;
		count++;
		return true;
	}

	// Searching each node after the first node, inserting before the first node with a lower priority
	Node<T> *currPtr = frontPtr;
	while (currPtr->getNext())
	{
		if (isGreaterThan(newNodePtr->getItem(), currPtr->getNext()->getItem()))
		{
			newNodePtr->setNext(currPtr->getNext());
			currPtr->setNext(newNodePtr);
			count++;
			return true;
		}
		currPtr = currPtr->getNext();
	}

	// If the new node has a lower priority than the back node
	backPtr->setNext(newNodePtr);
	backPtr = newNodePtr;
	count++;
	return true;
} // end enqueue


/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Function: dequeue
Removes the front of this PriorityQueue. That is, removes the item that was added
earliest.

Input: None.
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool PriorityQueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// PriorityQueue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: one node in PriorityQueue
		backPtr = nullptr;

	// Free memory reserved by the dequeued node
	delete nodeToDeletePtr;

	count--;
	return true;

}

/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: peekFront
gets the front of this PriorityQueue. The operation does not modify the PriorityQueue.

Input: None.
Output: The front of the PriorityQueue.
return: false if PriorityQueue is empty
*/
template <typename T>
bool PriorityQueue<T>::peekFront(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}

template<typename T>
int PriorityQueue<T>::getLength() const
{
	return count;
}

template <typename T>
PriorityQueue<T>::~PriorityQueue()
{
	Node<T> *currPtr = frontPtr;
	while (frontPtr)
	{
		frontPtr = frontPtr->getNext();
		delete currPtr;
		currPtr = frontPtr;
	}
}

#endif