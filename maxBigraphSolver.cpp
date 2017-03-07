#include <iostream>
#include <queue>
#include <set>
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

Graph * MaxBigraphSolver::FindMaxBigraph(Graph & originalGraph)
{
	m_GraphStack.push(&originalGraph);

	while(!m_GraphStack.empty())
	{
		//cout << "Stack is not empty: Number of elements = " << m_GraphStack.size() << endl;
		Graph * graph = m_GraphStack.top();
		m_GraphStack.pop();


		if (graph->m_Edges.size() > m_BestGraph->m_Edges.size())
		{
			cout << "GraphStack::POP:TryingGraph: NumberOfGraphEdges: " << graph->m_Edges.size();
			cout << ", bestGraph:NumberOfEdges: " << m_BestGraph->m_Edges.size() << endl;
			if (TryMakeBigraph(*graph))
			{
				cout << "FOUND BETTER GRAPH: NumberOfEdges = " << graph->m_Edges.size() << endl;
				if (m_BestGraph != &originalGraph)
					delete m_BestGraph;
				m_BestGraph = graph;
				continue;
			}

			if (graph->m_Edges.size() - 1 > m_BestGraph->m_Edges.size())
			{
				for (unsigned i = 0; i < graph->m_Edges.size(); i++)
				{
					Graph * childGraph = new Graph(*graph);
					childGraph->RemoveEdge(i);
					//cout << "Adding graph to stack: NumberOfEdges: " << childGraph->m_Edges.size() << endl;
					m_GraphStack.push(childGraph);
				}
			}
		}

		if (graph != &originalGraph)
			delete graph;
	}

	return m_BestGraph;
}

/***
*<summary> Tries to color the graph with 2 colors. </summary>
*<param> The graph to color. </param>
*<return> True if it is possible to color the graph with 2 colors, otherwise false.</return>
***/
bool MaxBigraphSolver::TryMakeBigraph(Graph & graph) const
{
	queue<int> nodesToColor;
	set<int> processedNodes;

	graph.m_NodeColors[0] = White;
	nodesToColor.push(0);
	std::pair<std::set<int>::iterator,bool> ret;

	while(1)
	{
		//cout << "MaxBigraphSolver::TryMakeBigraph: size of processedNodes = " << processedNodes.size() << endl;
		int nodeIndex = nodesToColor.front();
		nodesToColor.pop();
		Color neighbourColor = graph.m_NodeColors[nodeIndex] == Black ? White : Black;
		
		//cout << "MaxBigraphSolver::TryMakeBigraph: Colorring neighbours of node " << nodeIndex << " (" << graph.m_NodeColors[nodeIndex] << ")";
		//cout << " to color: " << neighbourColor << endl; 
		if (!graph.ColorNeighbourNodes(nodeIndex, neighbourColor))
		{
			//cout << "MaxBigraphSolver::TryMakeBigraph: UNABLE TO COLOR GRAPH" << endl;
			return false;
		}

		for (int i = 0; i < graph.m_NumberOfNodes; i++)
		{
			if (graph.AreNeighbours(nodeIndex, i))
			{
				if (processedNodes.find(i) == processedNodes.end())
				{
					//cout << "MaxBigraphSolver::TryMakeBigraph: Pushing not processed node: " << i << " to nodeToColor queue" << endl;
					nodesToColor.push(i);
				}
			}
		}

		//cout << "MaxBigraphSolver::TryMakeBigraph: INSERTING NODE " << nodeIndex << " to processedNodes" << endl;
		ret = processedNodes.insert(nodeIndex);
		//cout << "MaxBigraphSolver::TryMakeBigraph: ret.second = " << ret.second << endl;
	
		if (nodesToColor.empty())
		{
			int notColoredNode = graph.GetFirstUncoloredNode(); 
			//cout << "MaxBigraphSolver::TryMakeBigraph: Nodes to color queue is empty. Index of first not yet colored node: " << notColoredNode << endl;
			if (notColoredNode < 0)
			{
				break;
			}
			else
			{
				graph.m_NodeColors[notColoredNode] = White;
				nodesToColor.push(notColoredNode);
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
