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
		m_GraphStack.push(graph);
	}

	cout << "Graphs in stack: " << m_GraphStack.size() << endl;
	while(!m_GraphStack.empty())
	{
		//cout << "Stack is not empty: Number of elements = " << m_GraphStack.size() << endl;
		Graph * graph = m_GraphStack.top();
		m_GraphStack.pop();

		if (PossiblyBetterGraph(*graph))
		{
			TryPossiblyBetterGraph(graph);
		}

		if (graph != m_OriginalGraph && graph != m_BestGraph)
			delete graph;
	}

	return m_BestGraph;
}

void MaxBigraphSolver::TryPossiblyBetterGraph(Graph * graph)
{
	//unsigned maximumNumberOfEdges = m_OriginalGraph->m_NumberOfNodes * m_OriginalGraph->m_NumberOfNodes / 4;
	cout << "GraphStack::POP:TryingGraph: NumberOfGraphEdges: " << graph->m_NumberOfEdgesCurrent << endl;
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