#include <iostream>
#include "maxBigraphSolver.h"

using namespace std;

Graph * MaxBigraphSolver::FindMaxBigraph(Graph & graph)
{
	
}

/***
*<summary> Tries to color the graph with 2 colors. </summary>
*<param> The graph to color. </param>
*<return> True if it is possible to color the graph with 2 colors, otherwise false.</return>
***/
bool MaxBigraphSolver::TryMakeBigraph(Graph & graph)
{
	for (int i = 0; i < graph.m_NumberOfNodes; i++)
	{
		if (m_NodeColors[i] == Undefined)
		{
			
		}
	}

	return true;
}

bool TryColorNode(Graph & graph, int nodeIndex)
{
	Color tmp = Undefined;

	for (int i = 0; i < graph.m_NumberOfNodes; i++)
	{
		
	}
}
