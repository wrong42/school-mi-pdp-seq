#include <iostream>
#include "maxBigraphSolver.h"

using namespace std;

Graph * MaxBigraphSolver::FindMaxBigraph(Graph & graph)
{
	if (TryMakeBigraph(graph))
	{
		return &graph;
	}

	// list<Edge*>::iterator it = m_Edges.begin(); it != m_Edges.end(); ++it
	for (unsigned i = 0; i < graph.m_Edges.size(); i++)
	{
		Graph * childGraph = new Graph(graph);
		graph.RemoveEdge(i);
	}	
}

void MaxBigraphSolver::FindMaxBigraphInternal(Graph & graph)
{
	
}

/***
*<summary> Tries to color the graph with 2 colors. </summary>
*<param> The graph to color. </param>
*<return> True if it is possible to color the graph with 2 colors, otherwise false.</return>
***/
bool MaxBigraphSolver::TryMakeBigraph(Graph & graph) const
{
	for (int i = 0; i < graph.m_NumberOfNodes; i++)
	{
		if (graph.m_NodeColors[i] == Undefined)
		{
			if (!TryColorNode(graph, i))
			{
				return false;
			}	
		}
	}

	return true;
}

bool MaxBigraphSolver::TryColorNode(Graph & graph, int nodeIndex) const
{
	Color tmp = Undefined;

	for (int i = 0; i < graph.m_NumberOfNodes; i++)
	{
		if (graph.AreNeighbours(nodeIndex, i)) 						// The nodes are neighbours
		{
			if (graph.m_NodeColors[i] != Undefined)					// The neighbour is already colored
			{
				if (tmp == Undefined)										// No other previously visited neighbour has been colored 
				{
					tmp = graph.m_NodeColors[i];
				}
				else																// There was another neighbour previously visited, that has been colored
				{
					if ( tmp != graph.m_NodeColors[i])					// If the previously visited neigbour does not have the same color as this neighbour, we can not color this node
					{
						return false;
					}
				}
			}
		}		
	}

	switch(tmp)
	{
		case Undefined:
			graph.m_NodeColors[nodeIndex] = Black;
		case Black:
			graph.m_NodeColors[nodeIndex] = White;
		case White:
			graph.m_NodeColors[nodeIndex] = Black;
	}

	return true;
}
