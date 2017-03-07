#include <iostream>
#include <queue>
#include <set>
#include "maxBigraphSolver.h"

using namespace std;

MaxBigraphSolver::MaxBigraphSolver() : m_BigraphMaker()
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
	m_OriginalGraph = &originalGraph;
	m_GraphStack.push(&originalGraph);
	

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
	unsigned maximumNumberOfEdges = m_OriginalGraph->m_NumberOfNodes * m_OriginalGraph->m_NumberOfNodes / 4;
	//cout << "GraphStack::POP:TryingGraph: NumberOfGraphEdges: " << graph->m_Edges.size();
	//cout << ", bestGraph:NumberOfEdges: " << m_BestGraph->m_Edges.size() << endl;
	
	if (graph->m_Edges.size() > maximumNumberOfEdges)
	{
		AddChildGraphsToStack(graph);
		AddProcessedGraph(graph);
		return;
	}

	if (GraphHasBeenProcessed(*graph))
	{
		return;
	}
	
	if (TryMakeBigraph(*graph))
	{
		AcceptBetterGraph(graph);
	}

	AddProcessedGraph(graph);
	AddChildGraphsToStack(graph);
}
void MaxBigraphSolver::AcceptBetterGraph(Graph * graph)
{
	cout << "FOUND BETTER GRAPH: NumberOfEdges = " << graph->m_Edges.size() << endl;

	if (m_BestGraph != m_OriginalGraph)
		delete m_BestGraph;
	m_BestGraph = graph;
}

void MaxBigraphSolver::AddProcessedGraph(Graph * graph)
{
	if (graph->m_MissingEdgesById.size() < MaximumNumberOfMissingEdges)
	{
		set<int> * missingEdges = new set<int>(graph->m_MissingEdgesById);
		m_MissingEdges.push_back(missingEdges);
		m_ProcessedGraphsByEdge.insert(*missingEdges);
	}
}

void MaxBigraphSolver::AddChildGraphsToStack(Graph * graph)
{
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

bool MaxBigraphSolver::PossiblyBetterGraph(const Graph & graph) const
{
	return graph.m_Edges.size() > m_BestGraph->m_Edges.size();
}

bool MaxBigraphSolver::GraphHasBeenProcessed(const Graph & graph) const
{
	if (graph.m_MissingEdgesById.size() > MaximumNumberOfMissingEdges)
		return false;

	return m_ProcessedGraphsByEdge.find(graph.m_MissingEdgesById) != m_ProcessedGraphsByEdge.end();
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