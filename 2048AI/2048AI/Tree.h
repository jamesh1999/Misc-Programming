#ifndef __TREE_INCLUDED__
#define __TREE_INCLUDED__

#include "stdafx.h"
#include "LinkedList.h"
#include <iostream>
#include <vector>

namespace Containers
{
	//Forward declaration
	template <typename T>
	class FixedTree;
	template <typename T>
	class DynamicTree;

	template <typename T>
	class FixedTreeNode
	{
		friend class FixedTree<T>;

		int child_count = 0;
		FixedTreeNode<T>* children = nullptr;

	public:
		T value;

		FixedTreeNode() = default;
		FixedTreeNode(const T& v) : value(v) {}

		void operator=(const T&);
		FixedTreeNode<T>& operator[](int);
		int size() const;
	};

	template <typename T>
	class FixedTree
	{
		FixedTreeNode<T>* children;
		int total_children = 0;

	public:
		FixedTree(unsigned, unsigned);
		FixedTree(unsigned, unsigned, const T&);
		~FixedTree();
		
		FixedTreeNode<T>& operator[](int);
	};

	template <typename T>
	class DynamicTreeNode : protected LinkedList<DynamicTreeNode<T>>
	{
	friend DynamicTree<T>;

		std::vector<ListNode<DynamicTreeNode<T>>*>* allocated;

	public:
		T value;

		DynamicTreeNode() = default;
		DynamicTreeNode(const T& v) : value(v) {}
		virtual ~DynamicTreeNode();

		void operator=(const T&);
		DynamicTreeNode<T>& operator[](int) override;
		int size() const override;
		void AddChild(T);
		void InsertChild(int, T);
		void RemoveChild(int);
	};

	template <typename T>
	class DynamicTree
	{
		DynamicTreeNode<T>* root;
		std::vector<ListNode<DynamicTreeNode<T>>*> allocated;

	public:
		DynamicTree();
		virtual ~DynamicTree();

		void Reserve(int);
		void Free();
		DynamicTreeNode<T>& GetRoot();
	};



	//Update value of node
	template <typename T>
	void FixedTreeNode<T>::operator=(const T& val)
	{
		value = val;
	}

	//Returns the child with a given index
	template <typename T>
	FixedTreeNode<T>& FixedTreeNode<T>::operator[](int index)
	{
		if (index >= child_count || index < 0)
		{
			std::cerr << "FixedTreeNode::RemoveAt() index out of range: " << index << std::endl;
			exit(1);
		}

		return children[index];
	}

	//Returns the total number of child nodes
	template <typename T>
	int FixedTreeNode<T>::size() const
	{
		return child_count;
	}



	//Create tree with a given depth and number of child nodes per node
	template <typename T>
	FixedTree<T>::FixedTree(unsigned depth, unsigned child_nodes)
	{
		total_children = 1;
		int nodes_at_level = 1;
		for (int i = 1; i < depth; ++i)
		{
			nodes_at_level *= child_nodes;
			total_children += nodes_at_level;
		}

		children = new FixedTreeNode<T>[total_children];

		//Assign child counts
		for (int i = 0; i < total_children - nodes_at_level; ++i)
			children[i].child_count = child_nodes;

		//Link nodes
		int assigned = 1;
		for (int i = 0; i < total_children; ++i)
		{
			children[i].children = children + assigned;
			assigned += children[i].child_count;
		}
	}

	//" With a default value for each node
	template <typename T>
	FixedTree<T>::FixedTree(unsigned depth, unsigned child_nodes, const T& default_value)
	{
		total_children = 1;
		int nodes_at_level = 1;
		for (int i = 1; i < depth; ++i)
		{
			nodes_at_level *= child_nodes;
			total_children += nodes_at_level;
		}

		children = new FixedTreeNode<T>[total_children];

		//Assign child counts
		for (int i = 0; i < total_children - nodes_at_level; ++i)
			children[i].child_count = child_nodes;

		//Link nodes
		int assigned = 1;
		for (int i = 0; i < total_children; ++i)
		{
			children[i].children = children + assigned;
			assigned += children[i].child_count;
		}

		//Assign values of all children
		for (int i = 0; i < total_children; ++i)
			children[i] = default_value;
	}
	
	template <typename T>
	FixedTree<T>::~FixedTree()
	{
		delete[] children;
	}

	//Index the whole tree
	//Can index tree or do [0][a][b][c]... for a specific branch
	template <typename T>
	FixedTreeNode<T>& FixedTree<T>::operator[](int index)
	{
		if(index >= total_children || index < 0)
		{
			std::cerr << "FixedTree::operator[] index out of range: " << index << std::endl;
			exit(1);
		}

		return children[index];
	}



	template <typename T>
	DynamicTreeNode<T>::~DynamicTreeNode()
	{
		while (first != nullptr)
		{
			allocated->push_back(first);
			first = first->next;
			allocated->back()->~ListNode<DynamicTreeNode<T>>();
		}
	}

	//Sets the value of the node
	template <typename T>
	void DynamicTreeNode<T>::operator=(const T& desired_value)
	{
		value = desired_value;
	}

	template <typename T>
	DynamicTreeNode<T>& DynamicTreeNode<T>::operator[](int index)
	{
		if (index >= count || index < 0)
		{
			std::cerr << "DynamicTreeNode::operator[] index out of range: " << index << std::endl;
			exit(-1);
		}

		return getNode(index)->value;
	}

	//Returns the total number of child nodes
	template <typename T>
	int DynamicTreeNode<T>::size() const
	{
		return count;
	}

	//Adds a child to the end of the list
	template <typename T>
	void DynamicTreeNode<T>::AddChild(T child_value)
	{
		ListNode<DynamicTreeNode<T>>* to_add;
		if (allocated->size() <= 0) 
			to_add = new ListNode<DynamicTreeNode<T>>;
		else
		{
			to_add = new ((*allocated)[allocated->size() - 1]) ListNode<DynamicTreeNode<T>>;
			allocated->pop_back();
		}
		to_add->value.value = child_value;
		to_add->value.allocated = allocated;

		++count;
		if (count == 1)
		{
			first = to_add;
			return;
		}

		getNode(count - 2)->next = to_add;
	}

	//Inserts a child at a given index
	template <typename T>
	void DynamicTreeNode<T>::InsertChild(int index, T child_value)
	{	
		if (index > count)
		{
			std::cerr << "DynamicTreeNode::InsertChild() index out of range: " << index << std::endl;
			exit(-1);
		}

		ListNode<DynamicTreeNode<T>>* to_add;
		if (allocated->size() <= 0)
			to_add = new ListNode<DynamicTreeNode<T>>;
		else
		{
			to_add = new ((*allocated)[allocated->size() - 1]) ListNode<DynamicTreeNode<T>>;
			allocated->pop_back();
		}
		to_add->value.value = child_value;
		to_add->value.allocated = allocated;

		if (index < count)
			to_add->next = getNode(index);

		++count;
		if (index == 0)
		{
			first = to_add;
			return;
		}

		getNode(index - 1)->next = to_add;
	}

	//Removes a child at an index
	template <typename T>
	void DynamicTreeNode<T>::RemoveChild(int index)
	{
		if (index >= count)
		{
			std::cerr << "DynamicTreeNode::RemoveChild() index out of range: " << index << std::endl;
			exit(-1);
		}

		--count;
		if (index == 0)
		{
			first->~ListNode<DynamicTreeNode<T>>();
			allocated->push_back(first);
			first = getNode(index + 1);
			return;
		}

		if (index == count)
		{
			ListNode<DynamicTreeNode<T>>* to_delete = getNode(index - 1)->next;
			to_delete->~ListNode<DynamicTreeNode<T>>();
			allocated->push_back(to_delete);
			return;
		}

		ListNode<DynamicTreeNode<T>>* preceding = getNode(index - 1);
		ListNode<DynamicTreeNode<T>>* to_delete = preceding->next;
		ListNode<DynamicTreeNode<T>>* buffer = to_delete->next;

		to_delete->~ListNode<DynamicTreeNode<T>>();
		allocated->push_back(to_delete);
		preceding->next = buffer;
	}



	template <typename T>
	DynamicTree<T>::DynamicTree()
	{
		root = new DynamicTreeNode<T>;
		root->allocated = &allocated;
	}

	//Clean up allocated memory
	template <typename T>
	DynamicTree<T>::~DynamicTree()
	{
		delete root;
		Free();
	}

	template <typename T>
	DynamicTreeNode<T>& DynamicTree<T>::GetRoot()
	{
		return *root;
	}

	//Reserve memory for n nodes
	template <typename T>
	void DynamicTree<T>::Reserve(int count)
	{
		while(allocated.size() < count)
		{
			ListNode<DynamicTreeNode<T>>* new_node = new ListNode<DynamicTreeNode<T>>;
			allocated.push_back(new_node);
		}
	}

	//Free all allocated memory
	template <typename T>
	void DynamicTree<T>::Free()
	{
		for (auto val : allocated)
			delete val;
		allocated.clear();
	}
}

#endif