#include <iostream>
#include "maxBigraphSolver.h"

using namespace std;

MaxBigraphSolver::MaxBigraphSolver()
{
	m_BestGraph = new Graph(0);
}

MaxBigraphSolver::~MaxBigraphSolver()
{
	//if (m_BestGraph->m_NumberOfNodes == 0)
	//	delete m_BestGraph;
}

Graph * MaxBigraphSolver::FindMaxBigraph(Graph & graph)
{
	if (TryMakeBigraph(graph))
	{
		cout << "MaxBigraphSolver::FindMaxBigraph: Original graph is a bigraph" << endl;
		return &graph;
	}

	m_OriginalGraph = &graph;

	for (unsigned i = 0; i < graph.m_Edges.size(); i++)
	{
		Graph * childGraph = new Graph(graph);
		childGraph->RemoveEdge(i);
		FindMaxBigraphInternal(*childGraph);
		if (childGraph != m_BestGraph)
		{
			delete childGraph;
		}
	}	
	
	return m_BestGraph;
}

void MaxBigraphSolver::FindMaxBigraphInternal(Graph & graph)
{
	unsigned minimumNumberOfEdges = graph.m_NumberOfNodes - 1;
	
	while (graph.m_Edges.size() > minimumNumberOfEdges)
	{
		if (m_BestGraph->m_Edges.size() >= graph.m_Edges.size())
		{
			return;
		}
		else if (TryMakeBigraph(graph))
		{
			if (m_BestGraph->m_Edges.size() < graph.m_Edges.size())
			{
				if (m_BestGraph != m_OriginalGraph)
					delete m_BestGraph;
				m_BestGraph = &graph;
				return;
			}

			return;
		}
	
		graph.RemoveOneEdge();
	}
}

/***
*<summary> Tries to color the graph with 2 colors. </summary>
*<param> The graph to color. </param>
*<return> True if it is possible to color the graph with 2 colors, otherwise false.</return>
***/
bool MaxBigraphSolver::TryMakeBigraph(Graph & graph) const
{
	//cout << "MaxBigraphSolver.TryMakeBigraph: coloring " << graph.m_NumberOfNodes << " nodes" << endl;
	
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
	
	//cout << "MaxBigraphSolver::TryColorNode: Coloring node: " << nodeIndex << endl;
	for (int i = 0; i < graph.m_NumberOfNodes; i++)
	{
		if (graph.AreNeighbours(nodeIndex, i)) 						// The nodes are neighbours
		{
			//cout << "MaxBigraphSolver::TryColorNode: Found neighbour with index: " << i << " and color: " << graph.m_NodeColors[i] << endl;
			if (graph.m_NodeColors[i] != Undefined)					// The neighbour is already colored
			{
				if (tmp == Undefined)								// No other previously visited neighbour has been colored 
				{
					//cout << "MaxBigraphSolver::TryColorNode: Setting neighbours color to: " << graph.m_NodeColors[i] << endl;
					tmp = graph.m_NodeColors[i];
				}
				else												// There was another neighbour previously visited, that has been colored
				{
					if ( tmp != graph.m_NodeColors[i])				// If the previously visited neigbour does not have the same color as this neighbour, we can not color this node
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
			//cout << "MaxBigraphSolver::TryColorNode: Undef: neighbours color is " << tmp << ", setting to: " << Black << endl;
			graph.m_NodeColors[nodeIndex] = Black;
			break;
		case Black:
			//cout << "MaxBigraphSolver::TryColorNode: Black: neighbours color is " << tmp << ", setting to: " << White << endl;
			graph.m_NodeColors[nodeIndex] = White;
			break;
		case White:
			//cout << "MaxBigraphSolver::TryColorNode: White: neighbours color is " << tmp << ", setting to: " << Black << endl;
			graph.m_NodeColors[nodeIndex] = Black;
	}

	//cout << "Node "<< nodeIndex << " successfully colored with color: " << graph.m_NodeColors[nodeIndex] << endl;
	return true;
}
