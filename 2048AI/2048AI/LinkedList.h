#ifndef __LINKEDLIST_INCLUDED__
#define __LINKEDLIST_INCLUDED__

#include "stdafx.h"
#include <iostream>

namespace Containers
{
	template <typename T>
	struct ListNode
	{
		ListNode* next = nullptr;
		T value;
	};

	template <typename T>
	class LinkedList
	{
	protected:
		int count = 0;
		ListNode<T>* first = nullptr;

		ListNode<T>* getNode(int);

	public:
		virtual ~LinkedList();

		virtual void Add(const T&);
		virtual void RemoveAt(int);
		virtual void InsertAt(int, const T&);
		template<typename F> int FindIndex(F);
		template<typename F> T* Find(F);
		virtual int size() const;
		virtual T& operator[](int);
	};



	//Cleanup linked list
	template <typename T>
	LinkedList<T>::~LinkedList()
	{
		while (first != nullptr)
		{
			ListNode<T>* buffer = first->next;
			delete first;
			first = buffer;
		}
	}

	//Adds a new item to the list
	template <typename T>
	void LinkedList<T>::Add(const T& item)
	{

		ListNode<T>* to_add = new ListNode<T>;
		to_add->value = item;

		++count;
		if(count == 1)
		{
			first = to_add;
			return;
		}

		getNode(count - 2)->next = to_add;
	}

	//Removes an item at an index
	template <typename T>
	void LinkedList<T>::RemoveAt(const int index)
	{
		if(index >= count || index < 0)
		{
			std::cerr << "LinkedList::RemoveAt() index out of range: " << index << std::endl;
			exit(-1);
		}

		--count;
		if(index == 0)
		{
			delete first;
			first = getNode(index + 1);
			return;
		}

		if (index == count)
		{
			delete getNode(index - 1)->next;
			return;
		}

		ListNode<T>* preceding = getNode(index - 1);
		ListNode<T>* buffer = preceding->next->next;
		delete preceding->next;
		preceding->next = buffer;
	}

	//Inserts an item at a given index
	template <typename T>
	void LinkedList<T>::InsertAt(const int index, const T& item)
	{
		if(index > count || index < 0)
		{
			std::cerr << "LinkedList::InsertAt() index out of range: " << index << std::endl;
			exit(-1);
		}

		ListNode<T>* to_add = new ListNode<T>;
		to_add->value = item;

		if(index < count)
			to_add->next = getNode(index);

		++count;
		if(index == 0)
		{
		   first = to_add;
		   return;
		}

		getNode(index - 1)->next = to_add;
	}

	//Finds the index of the first item that satisfies a function
	//Returns -1 on failure
	template <typename T>
	template <typename F>
	int LinkedList<T>::FindIndex(F check)
	{
		int index = 0;

		ListNode<T>* current = first;

		while (current != nullptr)
		{
			if(check(current->value))
				return index;

			++index;
			current = current->next;
		}

		return -1;
	}

	//Finds the first item that satisfies a function
	//Returns nullptr on failure
	template <typename T>
	template <typename F>
	T* LinkedList<T>::Find(F check)
	{
		ListNode<T>* current = first;

		while (current != nullptr)
		{
			if(check(current->value))
				return &(current->value);

			current = current->next;
		}

		return nullptr;
	}

	//Returns the size of the linked list
	template <typename T>
	int LinkedList<T>::size() const
	{
		return count;
	}

	//Returns the item at a given index
	template <typename T>
	T& LinkedList<T>::operator[](int index)
	{
		if(index >= count || index < 0)
		{
			std::cerr << "LinkedList::operator[] index out of range: " << index << std::endl;
			exit(-1);
		}

		return getNode(index)->value;
	}

	//Returns the node at a given index
	template <typename T>
	ListNode<T>* LinkedList<T>::getNode(int index)
	{
		ListNode<T>* current = first;

		while (index-- > 0)
			current = current->next;

		return current;
	}
}

#endif // LINKEDLIST_H
