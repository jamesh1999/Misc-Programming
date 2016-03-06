#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "linkedlist.h"
#include <utility>
#include <climits>
#include <vector>
#include <iostream>

namespace Containers
{

template <class T1, class T2>
class HashTable
{
    LinkedList<std::pair<T1, T2>>* lists = nullptr;
    int list_count;

public:
    HashTable(int);
    ~HashTable();

    unsigned (*hashFunction)(const T1&);
    bool (*comparisonFunction)(const T1&, const T1&);
    T2 defaultValue;
    int size();
    int CompleteSize();
	void ResizeTable(int);
    bool DoesContain(const T1&);
    T2& operator[](const T1&);
    void RemoveKey(const T1&);
    std::vector<T1> GetKeys();

    //Get performance
    unsigned WorstCase();
    double AverageCase();
    unsigned BestCase();
};



//Construct new hash table with x many linked lists
template <class T1, class T2>
HashTable<T1, T2>::HashTable(int x) : list_count(x)
{
    lists = new LinkedList<std::pair<T1, T2>>[x];
}

//Clean up lists
template <class T1, class T2>
HashTable<T1, T2>::~HashTable()
{
    delete[] lists;
}

//Returns number of linked lists
template <class T1, class T2>
int HashTable<T1, T2>::size()
{
    return list_count;
}

//Returns total number of items in the table
template <class T1, class T2>
int HashTable<T1, T2>::CompleteSize()
{
    int total = 0;
    for(int i = 0; i < list_count; ++i)
        total += lists[i].size();
    return total;
}

//Resizes the table to contain a new number of lists
template <class T1, class T2>
void HashTable<T1, T2>::ResizeTable(int x)
{
	LinkedList<std::pair<T1, T2>>* new_lists = new LinkedList<std::pair<T1, T2>>[x];

	//Copy over values
	for (int i = 0; i < list_count; ++i)
		for (int j = 0; j < lists[i].size(); ++j)
		{
            unsigned new_list = hashFunction(lists[i][j].first) % x;
			std::pair<T1, T2> item(lists[i][j].first, lists[i][j].second);
			new_lists[new_list].InsertAt(0, item);
		}

	//Clean up
	delete[] lists;
	lists = new_lists;
	list_count = x;
}

//Get a pointer to the value with a given key
template <class T1, class T2>
T2& HashTable<T1, T2>::operator[](const T1& key)
{
    unsigned list = hashFunction(key) % list_count;
    std::pair<T1, T2>* val = lists[list].Find([&](std::pair<T1, T2> a)->bool{return comparisonFunction(a.first, key);});

    //If key is not in the table add it
    if(val == nullptr)
    {
        std::pair<T1, T2> blankItem(key, defaultValue);
        //Insert at beginning for better performance & access times
        lists[list].InsertAt(0, blankItem);
        //Get the last item
        val = &lists[list][lists[list].size() - 1];
    }

    return val->second;
}

//Checks for a value with a given key
template <class T1, class T2>
bool HashTable<T1, T2>::DoesContain(const T1& key)
{
    unsigned list = hashFunction(key) % list_count;
    std::pair<T1, T2>* val = lists[list].Find([&](std::pair<T1, T2> a)->bool{return comparisonFunction(a.first, key);});
    return val != nullptr;
}

//Constructs a vector of all keys in the hash table
template <class T1, class T2>
std::vector<T1> HashTable<T1, T2>::GetKeys()
{
    std::vector<T1> keys;
    keys.reserve(CompleteSize());

    for(int i = 0; i < list_count; ++i)
        for(int j = 0; j < lists[i].size(); ++j)
            keys.push_back(lists[i][j].first);

    return keys;
}

//Computes the worst case lookup for the hash table
template <class T1, class T2>
unsigned HashTable<T1, T2>::WorstCase()
{
    unsigned maxLength = 0;
    for(int i = 0; i < list_count; ++i)
        if(lists[i].size() > maxLength)
            maxLength = lists[i].size();
    return maxLength;
}

//Computes the average case lookup for the hash table
template <class T1, class T2>
double HashTable<T1, T2>::AverageCase()
{
    double totalLookup = 0;
    unsigned occupiedLists = 0;
    for(int i = 0; i < list_count; ++i)
        if(lists[i].size() > 0)
        {
            ++occupiedLists;
            totalLookup += (lists[i].size() + 1) / 2.0f;
        }
    return totalLookup / occupiedLists;
}

//Computes the best case lookup for the hash table
template <class T1, class T2>
unsigned HashTable<T1, T2>::BestCase()
{
    unsigned minLength = UINT_MAX;
    for(int i = 0; i < list_count; ++i)
        if(lists[i].size() < minLength && lists[i].size() > 0)
            minLength = lists[i].size();
    return minLength;
}

//Removes a given key and value from the hash table
template <class T1, class T2>
void HashTable<T1, T2>::RemoveKey(const T1& key)
{
    unsigned list = hashFunction(key) % list_count;
    int index = lists[list].FindIndex([&](std::pair<T1, T2> a)->bool{return comparisonFunction(a.first, key);});
    if(index == -1)
    {
        std::cerr << "HashTable::RemoveKey() key is not in hash table" << std::endl;
        exit(-1);
    }
    lists[list].RemoveAt(index);
}
}

#endif // HASHTABLE_H
