#ifndef __MIXEDLIST_H_
#define __MIXEDLIST_H_

#include "Node.h"

// 1st idea:
//		A combination between a List and a Queue that is tailored to fit operations
//		on Normal orders, such as Cancellation and Promotion;

template <typename T>
class MixedList
{
	Node<T>* frontPtr;
	Node<T>* backPtr;
public:
	MixedList();
	bool isEmpty() const;
	void append(T& newEntry);
	bool peekFront(T& frontEntry);
	bool pop(T& frontEntry);
	bool remove(int id, T& removedEntry);
	void clear();
	bool getEntry(int id, T& entryOfID);
};


template<typename T>
MixedList<T>::MixedList()
{
	frontPtr = backPtr = NULL;
}


template<typename T>
bool MixedList<T>::isEmpty() const
{
	return headPtr;
}


template<typename T>
void MixedList<T>::append(T & newEntry)
{
	Node<T>* newNodePtr = new Node<T>(newEntry);
	// Insert the new node
	if (isEmpty())
	{
		frontPtr = newNodePtr; // The queue is empty
		backPtr = frontPtr;
		return true;
	}
	else
	{
		backPtr->setNext(newNodePtr); // The queue was not empty
		backPtr = newNodePtr; // New node is at back
		return true;
	}
	return false;
}

template<typename T>
bool MixedList<T>::peekFront(T & frontEntry)
{
	if (isEmpty())
		return false;
	frontEntry = frontPtr->getItem();
	return true;
}


template<typename T>
bool MixedList<T>::pop(T& frontEntry)
{
	if (isEmpty())
		return false;

	Node<T>* toRemovePtr = frontPtr;
	frontEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	if (toRemovePtr == backPtr)	 // Special case: one node in queue
		backPtr = nullptr;

	// Free memory reserved by the dequeued node
	delete toRemovePtr;
	return true;
}


template<typename T>
bool MixedList<T>::remove(int id, T & removedEntry)
{
	if (isEmpty())
		return false;

	Node<T>* currPtr = frontPtr;
	if (currPtr->getItem() == id)
	{
		frontPtr = frontPtr->getNext();
		removedEntry = currPtr->getItem();
		delete currPtr;
		return true;
	}

	while (currPtr->getNext())
	{
		if (currPtr->getNext()->getItem() == id)
		{
			Node<T>* temp = currPtr->getNext();
			removedEntry = temp->getItem();
			currPtr->setNext(temp->getNext());
			if (backPtr == temp)
				backPtr = currPtr;
			delete temp;
			return true;
		}
		currPtr = currPtr->getNext();
	}

	return false;
}


template<typename T>
void MixedList<T>::clear()
{
	T clearPlace;
	/*while (!isEmpty())
		pop(clearPlace);*/
	while (pop(clearPlace));
}


template<typename T>
bool MixedList<T>::getEntry(int id, T& entryOfID)
{
	if (isEmpty())
		return false;

	Node<T>* currPtr = headPtr;

	while (currPtr)
	{
		if (currPtr->getItem() == id)
		{
			entryOfID = currPtr->getItem();
			return true;
		}
		currPtr = currPtr->getNext();
	}
	return false;
}

#endif