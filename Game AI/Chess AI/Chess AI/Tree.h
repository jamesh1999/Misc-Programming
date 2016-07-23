#ifndef __TREE_INCLUDED__
#define __TREE_INCLUDED__

#include "stdafx.h"
#include "LinkedList.h"
#include <iostream>

namespace Containers
{
	//Forward declaration
	template <typename T>
	class FixedTree;

	template <typename T>
	class FixedTreeNode
	{
		friend class FixedTree<T>;

		int child_count = 0;
		FixedTreeNode<T>* children = nullptr;

	public:
		T value;

		void operator=(const T&);
		FixedTreeNode<T>& operator[](int);
		int size();
	};

	template <typename T>
	class FixedTree
	{
		FixedTreeNode<T>* children;
		int total_children = 0;

	public:
		FixedTree(unsigned, unsigned);
		FixedTree(unsigned, unsigned, T);
		~FixedTree();

		FixedTreeNode<T>& operator[](int);
	};

	template <typename T>
	class DynamicTreeNode
	{
		LinkedList<DynamicTreeNode<T>> children;

	public:
		T value;

		void operator=(const T&);
		DynamicTreeNode<T>& operator[](int);
		int size();
		void AddChild(T);
		void InsertChild(int, T);
		void RemoveChild(int);
		template<typename F> int FindIndex(F);
		template<typename F> T* FindChild(F);
	};

	template <typename T>
	class DynamicTree
	{
		DynamicTreeNode<T>* root;

	public:
		DynamicTree();
		~DynamicTree();

		DynamicTreeNode<T>& GetRoot();
	};



	//Sets the value of the node
	template <typename T>
	void FixedTreeNode<T>::operator=(const T& desired_value)
	{
		value = desired_value;
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
	int FixedTreeNode<T>::size()
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
	FixedTree<T>::FixedTree(unsigned depth, unsigned child_nodes, T default_value)
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



	//Sets the value of the node
	template <typename T>
	void DynamicTreeNode<T>::operator=(const T& desired_value)
	{
		value = desired_value;
	}

	//Returns the child with a given index
	template <typename T>
	DynamicTreeNode<T>& DynamicTreeNode<T>::operator[](int index)
	{
		if (index >= children.size() || index < 0)
		{
			std::cerr << "DynamicTreeNode::RemoveAt() index out of range: " << index << std::endl;
			exit(1);
		}

		return children[index];
	}

	//Returns the total number of child nodes
	template <typename T>
	int DynamicTreeNode<T>::size()
	{
		return children.size();
	}

	//Adds a child to the end of the list
	template <typename T>
	void DynamicTreeNode<T>::AddChild(T child_value)
	{
		DynamicTreeNode<T> new_node;
		new_node.value = child_value;
		children.Add(new_node);
	}

	//Inserts a child at a given index
	template <typename T>
	void DynamicTreeNode<T>::InsertChild(int index, T child_value)
	{
		DynamicTreeNode<T> new_node;
		new_node.value = child_value;
		children.InsertAt(index, new_node);
	}

	//Removes a child at an index
	template <typename T>
	void DynamicTreeNode<T>::RemoveChild(int index)
	{
		children.RemoveAt(index);
	}

	//Finds the index of the child where check returns true
	template <typename T>
	template <typename F>
	int DynamicTreeNode<T>::FindIndex(F check)
	{
		return children.FindIndex(check);
	}

	//Finds the child where check returns true
	template <typename T>
	template <typename F>
	T* DynamicTreeNode<T>::FindChild(F check)
	{
		return children.Find(check);
	}



	template <typename T>
	DynamicTree<T>::DynamicTree()
	{
		root = new DynamicTreeNode<T>;
	}

	template <typename T>
	DynamicTree<T>::~DynamicTree()
	{
		delete root;
	}

	template <typename T>
	DynamicTreeNode<T>& DynamicTree<T>::GetRoot()
	{
		return *root;
	}
}

#endif