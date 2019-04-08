#ifndef __LINKEDLIST_H_
#define __LINKEDLIST_H_

#include "Node.h"

// 2nd idea:
//		A normal List ADT to handle orders.
// TODO:
//		Implement functions to operate on the List via an index.
//      cpy cons
//		priv getNodeAt

template <typename T>
class LinkedList
{
	Node<T>* frontPtr;
	Node<T>* backPtr;
	int count;
public:
	LinkedList();
	LinkedList(const LinkedList &copiedList);
	void operator=(const LinkedList &copiedList);
	int getLength() const;
	bool isEmpty() const;
	void clear();
	~LinkedList();

	void append(T& newEntry);
	bool pop(T& frontEntry);
	bool peekFront(T& frontEntry);
	
	//Removes by element, returns false if the element wasn't found in the List
	bool remove(T& removedEntry);	
};


template<typename T>
LinkedList<T>::LinkedList()
{
	frontPtr = backPtr = NULL;
	count = 0;
}

template<typename T>
LinkedList<T>::LinkedList(const LinkedList & copiedList)
{
	*this = copiedList;
}

template<typename T>
void LinkedList<T>::operator=(const LinkedList & copiedList)
{
	frontPtr = backPtr = NULL;
	count = 0;

	if (copiedList.isEmpty())
		return;
	
	Node<T> *copiedPtr = copiedList.frontPtr;
	backPtr = frontPtr = new Node<T>(copiedPtr->getItem());
	while (copiedPtr->getNext())
	{
		copiedPtr = copiedPtr->getNext();
		T added = copiedPtr->getItem();
		append(added);
	}
}

template<typename T>
int LinkedList<T>::getLength() const
{
	return count;
}


template<typename T>
bool LinkedList<T>::isEmpty() const
{
	return frontPtr == NULL;
}


template<typename T>
void LinkedList<T>::append(T & newEntry)
{
	Node<T>* newNodePtr = new Node<T>(newEntry);
	// Insert the new node
	if (isEmpty())
	{
		frontPtr = newNodePtr; // The queue is empty
		backPtr = frontPtr;
		count++;
	}
	else
	{
		backPtr->setNext(newNodePtr); // The queue was not empty
		backPtr = newNodePtr; // New node is at back
		count++;
	}


}

template<typename T>
bool LinkedList<T>::peekFront(T & frontEntry)
{
	if (isEmpty())
		return false;
	frontEntry = frontPtr->getItem();
	return true;
}


template<typename T>
bool LinkedList<T>::pop(T& frontEntry)
{
	if (isEmpty())
		return false;

	Node<T>* toRemovePtr = frontPtr;
	frontEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();		
	// Queue is not empty, remove front
	if (toRemovePtr == backPtr)	 // Special case: one node in queue
		backPtr = nullptr;

	// Free memory reserved by the dequeued node
	delete toRemovePtr;
	count--;
	return true;
}


template<typename T>
void LinkedList<T>::clear()
{
	T clearPlace;
	/*while (!isEmpty())
		pop(clearPlace);*/
	while (pop(clearPlace));
	count = 0;
}

template<typename T>
LinkedList<T>::~LinkedList()
{
	clear();
}

template<typename T>
bool LinkedList<T>::remove(T & removedEntry)
{
	if (isEmpty())
		return false;

	Node<T> *curPtr = frontPtr;
	if (curPtr->getItem() == removedEntry)
	{
		frontPtr = frontPtr->getNext();					
		if (!frontPtr) backPtr = frontPtr;
		curPtr->setNext(NULL);
		delete curPtr;
		count--;
		return true;
	}

	while (curPtr->getNext())
	{
		if (curPtr->getNext()->getItem() == removedEntry)
		{
			Node<T>* deletedPtr = curPtr->getNext();
			curPtr->setNext(deletedPtr->getNext());
			if (backPtr == deletedPtr)						
				backPtr = curPtr;
			deletedPtr->setNext(NULL);
			delete deletedPtr;
			count--;
			return true;
		}
		curPtr = curPtr->getNext();
	}
	return false;
}

#endif
