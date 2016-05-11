#ifndef __AITREE_INCLUDED__
#define __AITREE_INCLUDED__

#include "stdafx.h"
#include <iostream>
#include "Tree.h"

namespace AI
{

	template <typename T>
	struct MinMaxData
	{
		T board;
		double alpha = DBL_MIN;
		double beta = DBL_MAX;
		double score = DBL_MIN;

		void Clear() { alpha = DBL_MIN; beta = DBL_MAX; score = DBL_MIN; }
	};

	template <typename T>
	class MinMaxTree : Containers::DynamicTree<MinMaxData<T>>
	{
	protected:
		virtual void RecalculateRecursive(Containers::DynamicTreeNode<MinMaxData<T>>*, int);

	public:
		double(*evaluationFunction)(const T&);
		int(*getPlayerMovesFunction)(const T&, T*&);
		int(*getOpponentMovesFunction)(const T&, T*&);
		int maxDepth;
		double maxTime;
		double nullCutoff;

		T Recalculate(const T&);
		T GetBest();
		void ClearTree();
	};



	//Updates a single node with new state & calls child nodes
	template <typename T>
	void MinMaxTree<T>::RecalculateRecursive(Containers::DynamicTreeNode<MinMaxData<T>>* working_node, int depth)
	{
		//Don't go over max depth: Evaluate & return
		if (depth >= maxDepth)
		{
			working_node->value.score = evaluationFunction(working_node->value.board);
			return;
		}

		//Create all children
		T* child_boards = nullptr;
		int total_boards = 0;

		if (depth % 2)
			total_boards = getOpponentMovesFunction(working_node->value.board, child_boards);
		else
			total_boards = getPlayerMovesFunction(working_node->value.board, child_boards);

		//Calculate children
		for (int i = 0; i < total_boards; ++i)
		{
			//Create and Add child node
			MinMaxData<T> data;
			data.board = child_boards[i];
			data.alpha = working_node->value.alpha;
			data.beta = working_node->value.beta;
			data.score = depth % 2 ? DBL_MIN : DBL_MAX; //Set starting score dependant on min/max node
			working_node->InsertChild(0, data);

			//Recalculate node
			RecalculateRecursive(&((*working_node)[0]), depth + 1);

			//Handle alpha/beta pruning & set score as required
			if (depth % 2)
			{
				//Update score
				if (working_node->value.score > (*working_node)[0].value.score)
					working_node->value.score = (*working_node)[0].value.score;

				//Beta set
				if (working_node->value.beta > (*working_node)[0].value.score)
					working_node->value.beta = (*working_node)[0].value.score;
			}
			else
			{
				//Update score
				if (working_node->value.score < (*working_node)[0].value.score)
					working_node->value.score = (*working_node)[0].value.score;

				//Alpha set
				if (working_node->value.alpha < (*working_node)[0].value.score)
					working_node->value.alpha = (*working_node)[0].value.score;
			}

			//Node will have no effect => skip remaining children
			if (working_node->value.alpha > working_node->value.beta)
				break;
		}

		//Clean up
		delete[] child_boards;

		//If leaf node: evaluate
		if(total_boards == 0)
			working_node->value.score = evaluationFunction(working_node->value.board);
	}

	//Updates the MinMax tree with a new game state and calculates the best move
	template <typename T>
	T MinMaxTree<T>::Recalculate(const T& new_board)
	{
		ClearTree();
		
		//Update with new state
		GetRoot().value.board = new_board;

		//Recursively calculate nodes
		RecalculateRecursive(&GetRoot(), 0);

		//Return best
		return GetBest();
	}

	//Returns the current best move
	template <typename T>
	T MinMaxTree<T>::GetBest()
	{
		if(GetRoot().size() == 0)
		{
			std::cerr << "MinMaxTree::GetBest() no tree exists!" << std::endl;
			exit(-1);
		}

		double max = DBL_MIN;
		int max_index = 0;
		for (int i = 0; i < GetRoot().size(); ++i)
			if (GetRoot()[i].value.score > max)
			{
				max_index = i;
				max = GetRoot()[i].value.score;
			}

		return GetRoot()[max_index].value.board;
	}

	//Clears the current tree
	template <typename T>
	void MinMaxTree<T>::ClearTree()
	{
		while (GetRoot().size())
			GetRoot().RemoveChild(0);
		GetRoot().value.Clear();
	}
}

#endif