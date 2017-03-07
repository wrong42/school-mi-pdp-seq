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
	for (unsigned i = 0; i < m_MissingEdges.size(); i++)
		delete m_MissingEdges[i];
}

Graph * MaxBigraphSolver::FindMaxBigraph(Graph & originalGraph)
{
	m_GraphStack.push(&originalGraph);

	unsigned maximumNumberOfEdges = originalGraph.m_NumberOfNodes * originalGraph.m_NumberOfNodes / 4;

	while(!m_GraphStack.empty())
	{
		//cout << "Stack is not empty: Number of elements = " << m_GraphStack.size() << endl;
		Graph * graph = m_GraphStack.top();
		m_GraphStack.pop();


		if (PossiblyBetterGraph(*graph))
		{
			//cout << "GraphStack::POP:TryingGraph: NumberOfGraphEdges: " << graph->m_Edges.size();
			//cout << ", bestGraph:NumberOfEdges: " << m_BestGraph->m_Edges.size() << endl;
			
			if (graph->m_Edges.size() > maximumNumberOfEdges)
			{

			}
			else if (graph->m_MissingEdgesById.size() < 4)
			{
				if (GraphHasBeenProcessed(*graph))
				{
					if (graph != &originalGraph)
						delete graph;
					continue;
				}
				else if (TryMakeBigraph(*graph))
				{
					cout << "FOUND BETTER GRAPH: NumberOfEdges = " << graph->m_Edges.size() << endl;
					if (m_BestGraph != &originalGraph)
						delete m_BestGraph;
					m_BestGraph = graph;
					set<int> * missingEdges = new set<int>(graph->m_MissingEdgesById);
					m_MissingEdges.push_back(missingEdges);
					m_ProcessedGraphsByEdge.insert(*missingEdges);
					continue;
				}	
			}
			else if (TryMakeBigraph(*graph))
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

		if (graph->m_MissingEdgesById.size() < 4)
		{
			set<int> * missingEdges = new set<int>(graph->m_MissingEdgesById);
			m_MissingEdges.push_back(missingEdges);
			m_ProcessedGraphsByEdge.insert(*missingEdges);
		}

		if (graph != &originalGraph)
			delete graph;

	}

	return m_BestGraph;
}

bool MaxBigraphSolver::PossiblyBetterGraph(const Graph & graph) const
{
	return graph.m_Edges.size() > m_BestGraph->m_Edges.size();
}

bool MaxBigraphSolver::GraphHasBeenProcessed(const Graph & graph) const
{
	if (m_ProcessedGraphsByEdge.find(graph.m_MissingEdgesById) != m_ProcessedGraphsByEdge.end())
	{
		return true;
	}

	return false;
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

	/* Initialize queue with nodes to color. First node colored White */
	graph.m_NodeColors[0] = White;
	nodesToColor.push(0);

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

		processedNodes.insert(nodeIndex);
	
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