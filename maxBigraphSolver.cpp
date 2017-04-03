#include <iostream>
#include <queue>
#include <set>
#include <cstring>
#include <omp.h>
#include "maxBigraphSolver.h"

#define NUMBER_OF_THREADS 4

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

	if (m_BigraphMaker.MakeBigraph(originalGraph))
	{
		cout << "ORIGINAL GRAPH IS RESULT" << endl;
		return &originalGraph;
	}

	// Zkusit graf bez jedne hrany a pak pridat vsechny podgrafy
	Graph firstGraph(originalGraph);
	firstGraph.RemoveEdge(0);
	firstGraph.m_LastErasedEdge = 0;

	if (m_BigraphMaker.MakeBigraph(firstGraph))
	{
		return new Graph(firstGraph);
	}

	// First lvl edge removing from index 1
	//#pragma omp for schedule(static, originalGraph.m_NumberOfEdgesOriginal / NUMBER_OF_THREADS)
	for (int i = 1; i < originalGraph.m_NumberOfEdgesOriginal; i++)
	{
		//Graph * graph = new Graph(originalGraph);
		Graph graph(originalGraph);
		graph.RemoveEdge(i);
		graph.m_LastErasedEdge = i;
		m_Graphs.push_back(graph);
	}

	// Second lvl edge removing (edge 0 already removed) from index 1
	//#pragma omp barrier
	//#pragma omp for schedule(static, (originalGraph.m_NumberOfEdgesOriginal - 1)/ NUMBER_OF_THREADS)
	for (int i = 1; i < firstGraph.m_NumberOfEdgesOriginal; i++)
	{
		//Graph * graph = new Graph(originalGraph);
		Graph graph(firstGraph);
		graph.RemoveEdge(i);
		m_Graphs.push_back(graph);
	}

	#pragma omp parallel num_threads(NUMBER_OF_THREADS)
	{	
		//#pragma omp barrier
		#pragma omp for schedule(static,1)
		for (int i = 0; i < originalGraph.m_NumberOfEdgesOriginal; i++)
		{
			FindMaxBigraphInternal(m_Graphs[i]);
		}
		
	}

	return m_BestGraph;
}

void MaxBigraphSolver::FindMaxBigraphInternal(Graph graph)
{
	if (graph.m_NumberOfEdgesCurrent < graph.m_NumberOfNodes - 1)
	{
		return;
	}

	queue<int> m_ColoredNodes;
	set<int> m_ProcessedNodes;

	graph.m_NodeColors[0] = White;
	m_ColoredNodes.push(0);
	bool coloringSuccessful = true;

	while(!m_ColoredNodes.empty())
	{
		int nodeIndex = m_ColoredNodes.front();
		m_ColoredNodes.pop();

		Color neighbourColor = graph.m_NodeColors[nodeIndex] == Black ? White : Black;
		
		if (!graph.ColorNeighbourNodes(nodeIndex, neighbourColor))
		{
			//cout << "BigraphMaker::ColorNodes: UNABLE TO COLOR GRAPH" << endl;
			coloringSuccessful = false;
			break;
		}

		m_ProcessedNodes.insert(nodeIndex);

		for (int i = 0; i < graph.m_NumberOfNodes; i++)
		{
			if (graph.AreNeighbours(nodeIndex, i))
			{
				if (m_ProcessedNodes.find(i) == m_ProcessedNodes.end())
				{
					//cout << "BigraphMaker::ColorNodes: Pushing not processed node: " << i << " to nodeToColor queue" << endl;
					m_ColoredNodes.push(i);
				}
			}
		}

		if (m_ColoredNodes.empty())
		{
			int notColoredNode = graph.GetFirstUncoloredNode(); 
			//cout << "BigraphMaker::ColorNodes: Nodes to color queue is empty. Index of first not yet colored node: " << notColoredNode << endl;
			if (notColoredNode > -1)
			{
				graph.m_NodeColors[notColoredNode] = White;
				m_ColoredNodes.push(notColoredNode);
			}
		}
	} // End of graph coloring

	if (coloringSuccessful)
	{
		AcceptBetterGraph(&graph);
		return;
	}
	else if (m_ColoredNodes.size() == 0 && m_ProcessedNodes.size() < graph.m_NumberOfNodes)
	{
		return;
	}

	int numberOfBestGraphEdges = 0;
	
	#pragma omp atomic read
	numberOfBestGraphEdges = m_BestGraph->m_NumberOfEdgesCurrent;

	if (graph.m_NumberOfEdgesCurrent - 1 > numberOfBestGraphEdges)
	{
		for (unsigned i = graph.m_LastErasedEdge + 1; i < graph.m_NumberOfEdgesOriginal; i++)
		{
			//Graph * childGraph = new Graph(*graph);
			Graph childGraph(graph);
			childGraph.RemoveEdge(i);
			//cout << "Adding graph to stack: NumberOfEdges: " << childGraph->m_Edges.size() << endl;
			
			FindMaxBigraphInternal(childGraph);
	
		}
	}
}


void MaxBigraphSolver::AcceptBetterGraph(Graph * graph)
{
	cout << "FOUND BETTER GRAPH: NumberOfEdges = " << graph->m_NumberOfEdgesCurrent << endl;

	/*if (m_BestGraph != m_OriginalGraph)
		delete m_BestGraph;*/

	#pragma omp critical
	{
		delete m_BestGraph;
		m_BestGraph = new Graph(*graph);	
	}
}