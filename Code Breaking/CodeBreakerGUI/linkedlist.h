#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

namespace Containers
{
template <class T>
class Node
{
public:
    Node* next = nullptr;
    T value;
};

template <class T>
class LinkedList
{
    int count = 0;
    Node<T>* getNode(int);
public:
    ~LinkedList();

    Node<T>* first = nullptr;

    void Add(T);
    void RemoveAt(int);
    void InsertAt(int, T);
    template<typename F> int FindIndex(F);
    template<typename F> T* Find(F);
    int size();
    T& operator[](int);
};



//Cleanup linked list
template <class T>
LinkedList<T>::~LinkedList()
{
    while (first != nullptr)
    {
        Node<T>* buffer = first->next;
        delete first;
        first = buffer;
    }
}

//Adds a new item to the list
template <class T>
void LinkedList<T>::Add(T item)
{

    Node<T>* to_add = new Node<T>;
    to_add->value = item;

    if(count == 0)
    {
        first = to_add;
        ++count;
        return;
    }

    Node<T>* last = getNode(count - 1);
    last->next = to_add;
    ++count;
}

//Removes an item at an index
template <class T>
void LinkedList<T>::RemoveAt(const int index)
{
    if(index >= count)
    {
        std::cerr << "LinkedList::RemoveAt() index out of range: " << index << std::endl;
        exit(-1);
    }

    --count;
    if(index == count)
    {
        delete getNode(index - 1)->next;
        return;
    }

    Node<T>* buffer = getNode(index + 1);
    if(index == 0)
    {
        delete first;
        first = buffer;
        return;
    }

    Node<T>* preceding = getNode(index - 1);
    delete preceding->next;
    preceding->next = buffer;
}

//Inserts an item at a given index
template <class T>
void LinkedList<T>::InsertAt(const int index, T item)
{
    if(index > count)
    {
        std::cerr << "LinkedList::InsertAt() index out of range: " << index << std::endl;
        exit(-1);
    }

    Node<T>* to_add = new Node<T>;
    to_add->value = item;

    if(index < count)
    {
        Node<T>* buffer = getNode(index);
        to_add->next = buffer;
    }

    ++count;
    if(index == 0)
    {
       first = to_add;
       return;
    }

    Node<T>* preceding = getNode(index - 1);
    preceding->next = to_add;
}

//Finds the index of the first item that satisfies a function
//Returns -1 on failure
template <class T>
template<typename F>
int LinkedList<T>::FindIndex(F check)
{
    int index = -1;

    Node<T>* current = first;

    while (current != nullptr)
    {
        ++index;

        if(check(current->value))
            return index;

        current = current->next;
    }

    return -1;
}

//Finds the first item that satisfies a function
//Returns nullptr on failure
template <class T>
template<typename F>
T* LinkedList<T>::Find(F check)
{
    Node<T>* current = first;

    while (current != nullptr)
    {
        if(check(current->value))
            return &(current->value);

        current = current->next;
    }

    return nullptr;
}

//Returns the size of the linked list
template <class T> int LinkedList<T>::size()
{
    return count;
}

//Returns the item at a given index
template <class T>
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
template <class T>
Node<T>* LinkedList<T>::getNode(int index)
{
    Node<T>* current = first;

    while (index > 0)
    {
        --index;
        current = current->next;
    }

    return current;
}
}

#endif // LINKEDLIST_H
