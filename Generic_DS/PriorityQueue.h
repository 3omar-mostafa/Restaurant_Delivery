#ifndef __PRIORITYQUEUE_H_
#define __PRIORITYQUEUE_H_
#include "Node.h"

template <typename T>
class PriorityQueue
{
private:
	Node<T>* backPtr;
	Node<T>* frontPtr;
public:
	PriorityQueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry);
	bool dequeue(T& frntEntry);
	bool peekFront(T& frntEntry)  const;
	
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
		return true;
	}

	// If the new node has a higher priority than the front node
	if (newNodePtr->getItem() > frontPtr->getItem())
	{
		newNodePtr->setNext(frontPtr);
		frontPtr = newNodePtr;
		return true;
	}

	// Searching each node after the first node, inserting before the first node with a lower priority
	Node<T> *currPtr = frontPtr;
	while (currPtr->getNext())
	{
		if (newNodePtr->getItem() > currPtr->getNext()->getItem())
		{
			newNodePtr->setNext(currPtr->getNext());
			currPtr->setNext(newNodePtr);
			return true;
		}
		currPtr = currPtr->getNext();
	}

	// If the new node has a lower priority than the back node
	backPtr->setNext(newNodePtr);
	backPtr = newNodePtr;
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

template <typename T>
PriorityQueue<T>::~PriorityQueue()
{
}
#endif