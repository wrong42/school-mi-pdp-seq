#include <iostream>
#include <queue>
#include <set>
#include <cstring>
#include "maxBigraphSolver.h"

using namespace std;

MaxBigraphSolver::MaxBigraphSolver() : m_BigraphMaker()
{
}

MaxBigraphSolver::~MaxBigraphSolver()
{
}

Graph * MaxBigraphSolver::FindMaxBigraph(Graph & originalGraph)
{
	m_BestGraph = new Graph(0);
	m_BestGraph->m_Edges = originalGraph.m_Edges;
	m_BestGraph->m_NumberOfEdgesOriginal = originalGraph.m_NumberOfEdgesOriginal;
	m_BestGraph->m_NumberOfEdgesCurrent = 0;
	m_BestGraph->m_EdgeMatrix = new bool[originalGraph.m_NumberOfEdgesOriginal];
	memset((void*)m_BestGraph->m_EdgeMatrix, 0, originalGraph.m_NumberOfEdgesOriginal * sizeof(bool));

	cout << "Finding max bigraph" << endl;

	if (TryMakeBigraph(originalGraph))
	{
		cout << "ORIGINAL GRAPH IS RESULT" << endl;
		return &originalGraph;
	}

//	m_GraphStack.push(&originalGraph);

	m_OriginalGraph = &originalGraph;
	
	for (int i = 0; i < originalGraph.m_NumberOfEdgesOriginal; i++)
	{
		Graph * graph = new Graph(originalGraph);
		graph->RemoveEdge(i);
		FindMaxBigraphInternal(graph);
//		m_GraphStack.push(graph);
	}

/*	cout << "Graphs in stack: " << m_GraphStack.size() << endl;
	while(!m_GraphStack.empty())
	{
		//cout << "Stack is not empty: Number of elements = " << m_GraphStack.size() << endl;
		Graph * graph = m_GraphStack.top();
		m_GraphStack.pop();

		if (graph->m_NumberOfEdgesCurrent > m_BestGraph->m_NumberOfEdgesCurrent)
		{
			TryPossiblyBetterGraph(graph);
		}

		if (graph != m_OriginalGraph && graph != m_BestGraph)
			delete graph;
	}*/

	return m_BestGraph;
}

void MaxBigraphSolver::FindMaxBigraphInternal(Graph * graph)
{
	if (graph->m_NumberOfEdgesCurrent < graph->m_NumberOfNodes - 1)
	{
		if (graph != m_OriginalGraph && graph != m_BestGraph)
			delete graph;
		return;
	}
	

	queue<int> m_ColoredNodes;
	set<int> m_ProcessedNodes;

	graph->m_NodeColors[0] = White;
	m_ColoredNodes.push(0);
	bool coloringSuccessful = true;

	while(!m_ColoredNodes.empty())
	{
		int nodeIndex = m_ColoredNodes.front();
		m_ColoredNodes.pop();

		Color neighbourColor = graph->m_NodeColors[nodeIndex] == Black ? White : Black;
		
		if (!graph->ColorNeighbourNodes(nodeIndex, neighbourColor))
		{
			//cout << "BigraphMaker::ColorNodes: UNABLE TO COLOR GRAPH" << endl;
			coloringSuccessful = false;
			break;
		}

		m_ProcessedNodes.insert(nodeIndex);

		for (int i = 0; i < graph->m_NumberOfNodes; i++)
		{
			if (graph->AreNeighbours(nodeIndex, i))
			{
				if (m_ProcessedNodes.find(i) == m_ProcessedNodes.end())
				{
					//cout << "BigraphMaker::ColorNodes: Pushing not processed node: " << i << " to nodeToColor queue" << endl;
					m_ColoredNodes.push(i);
				}
			}
		}
	}

	if (coloringSuccessful)
	{
		AcceptBetterGraph(graph);
		return;
	}
	else if (m_ColoredNodes.size() == 0 && m_ProcessedNodes.size() < graph->m_NumberOfNodes)
	{
		if (graph != m_OriginalGraph && graph != m_BestGraph)
			delete graph;
		return;
	}

	if (graph->m_NumberOfEdgesCurrent - 1 > m_BestGraph->m_NumberOfEdgesCurrent)
	{
		for (unsigned i = graph->m_LastErasedEdge + 1; i < graph->m_NumberOfEdgesOriginal; i++)
		{
			Graph * childGraph = new Graph(*graph);
			childGraph->RemoveEdge(i);
			//cout << "Adding graph to stack: NumberOfEdges: " << childGraph->m_Edges.size() << endl;
			FindMaxBigraphInternal(childGraph);
		}
	}

	if (graph != m_OriginalGraph && graph != m_BestGraph)
		delete graph;
}

void MaxBigraphSolver::TryPossiblyBetterGraph(Graph * graph)
{
	//unsigned maximumNumberOfEdges = m_OriginalGraph->m_NumberOfNodes * m_OriginalGraph->m_NumberOfNodes / 4;
	//cout << "GraphStack::POP:TryingGraph: NumberOfGraphEdges: " << graph->m_NumberOfEdgesCurrent << endl;
	//cout << ", bestGraph:NumberOfEdges: " << m_BestGraph->m_Edges.size() << endl;
	
	if (graph->m_NumberOfEdgesCurrent < graph->m_NumberOfNodes - 1)
	{
		return;
	}
	
	if (TryMakeBigraph(*graph))
	{
		AcceptBetterGraph(graph);
		return;
	}
	else if (m_BigraphMaker.m_ColoredNodes.size() == 0 && m_BigraphMaker.m_ProcessedNodes.size() < graph->m_NumberOfNodes)
	{
		return;
	}

	AddChildGraphsToStack(graph);
}

void MaxBigraphSolver::AcceptBetterGraph(Graph * graph)
{
	cout << "FOUND BETTER GRAPH: NumberOfEdges = " << graph->m_NumberOfEdgesCurrent << endl;

	if (m_BestGraph != m_OriginalGraph)
		delete m_BestGraph;
	m_BestGraph = graph;
}

void MaxBigraphSolver::AddChildGraphsToStack(Graph * graph)
{
	if (graph->m_NumberOfEdgesCurrent - 1 > m_BestGraph->m_NumberOfEdgesCurrent)
	{
		for (unsigned i = graph->m_LastErasedEdge + 1; i < graph->m_NumberOfEdgesOriginal; i++)
		{
			Graph * childGraph = new Graph(*graph);
			childGraph->RemoveEdge(i);
			//cout << "Adding graph to stack: NumberOfEdges: " << childGraph->m_Edges.size() << endl;
			m_GraphStack.push(childGraph);
		}
	}
}

bool MaxBigraphSolver::PossiblyBetterGraph(const Graph & graph) const
{
	return graph.m_NumberOfEdgesCurrent > m_BestGraph->m_NumberOfEdgesCurrent;
}

/***
*<summary> Tries to color the graph with 2 colors. </summary>
*<param> The graph to color. </param>
*<return> True if it is possible to color the graph with 2 colors, otherwise false.</return>
***/
bool MaxBigraphSolver::TryMakeBigraph(Graph & graph)
{
	return m_BigraphMaker.MakeBigraph(graph);
}