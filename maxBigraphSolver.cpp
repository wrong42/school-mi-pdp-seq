#include <iostream>
#include "maxBigraphSolver.h"

using namespace std;

MaxBigraphSolver::MaxBigraphSolver()
{
	m_BestGraph = new Graph(0);
}

MaxBigraphSolver::~MaxBigraphSolver()
{
	delete m_BestGraph;
}

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
		FindMaxBigraphInternal(*childGraph);
	}	
	
	return m_BestGraph;
}

void MaxBigraphSolver::FindMaxBigraphInternal(Graph & graph)
{
	if (m_BestGraph->m_Edges.size() >= graph.m_Edges.size())
	{
		return;
	}
	else if (graph.m_Edges.size() == graph.m_NumberOfNodes - 1 || TryMakeBigraph(graph))
	{
		if (m_BestGraph->m_Edges.size() < graph.m_Edges.size())
		{
			delete m_BestGraph;
			m_BestGraph = &graph;
			return;
		}
	}
	
	for (unsigned i = 0; i < graph.m_Edges.size(); i++)
	{
		Graph * childGraph = new Graph(graph);
		graph.RemoveEdge(i);
		FindMaxBigraphInternal(*childGraph);
	}	
	
	if (&graph != m_BestGraph)
		delete &graph;
}

/***
*<summary> Tries to color the graph with 2 colors. </summary>
*<param> The graph to color. </param>
*<return> True if it is possible to color the graph with 2 colors, otherwise false.</return>
***/
bool MaxBigraphSolver::TryMakeBigraph(Graph & graph) const
{
	cout << "MaxBigraphSolver.TryMakeBigraph: coloring " << graph.m_NumberOfNodes << " nodes" << endl;
	
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
	
	cout << "MaxBigraphSolver::TryColorNode: Coloring node: " << nodeIndex << endl;
	for (int i = 0; i < graph.m_NumberOfNodes; i++)
	{
		if (graph.AreNeighbours(nodeIndex, i)) 						// The nodes are neighbours
		{
			cout << "MaxBigraphSolver::TryColorNode: Found neighbour with index: " << i << " and color: " << graph.m_NodeColors[i] << endl;
			if (graph.m_NodeColors[i] != Undefined)					// The neighbour is already colored
			{
				if (tmp == Undefined)								// No other previously visited neighbour has been colored 
				{
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
			graph.m_NodeColors[nodeIndex] = Black;
		case Black:
			graph.m_NodeColors[nodeIndex] = White;
		case White:
			graph.m_NodeColors[nodeIndex] = Black;
	}

	cout << "Node "<< nodeIndex << " successfully colored with color: " << graph.m_NodeColors[nodeIndex] << endl;
	return true;
}
