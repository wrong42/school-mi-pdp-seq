#include <iostream>
#include <cstring>
#include <queue>
#include <set>
#include <omp.h>
#include <mpi.h>
#include "graphLoader.h"
#include "bigraphMaker.h"
#include "graph.h"
#include "edge.h"

using namespace std;

#define NUMBER_OF_THREADS 4

enum MPI_TAGS {};


Graph * m_BestGraph;
vector<Graph> m_Graphs;
vector<Graph> m_FirstLvlGraphs;
BigraphMaker m_BigraphMaker;

void DeleteGraphs(Graph * loadedGraph, Graph * resultGraph)
{
	if (resultGraph != loadedGraph)
	{
		delete resultGraph;
	}
	
	for (unsigned i = 0; i < loadedGraph->m_NumberOfEdgesOriginal; i++)
	{
		delete loadedGraph->m_Edges[i];
	}
	
	delete[] loadedGraph->m_Edges;
	delete loadedGraph;
}

void InitializeBestGraph(Graph & originalGraph)
{
	m_BestGraph = new Graph(0);
	m_BestGraph->m_Edges = originalGraph.m_Edges;
	m_BestGraph->m_NumberOfEdgesOriginal = originalGraph.m_NumberOfEdgesOriginal;
	m_BestGraph->m_NumberOfEdgesCurrent = 0;
	m_BestGraph->m_EdgeMatrix = new bool[originalGraph.m_NumberOfEdgesOriginal];
	memset((void*)m_BestGraph->m_EdgeMatrix, 0, originalGraph.m_NumberOfEdgesOriginal * sizeof(bool));
}

void AcceptBetterGraph(Graph * graph)
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

void FindMaxBigraphInternal(Graph graph)
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

int main(int argc, char * argv[])
{
	MPI_Init(&argc, &argv);

	int proc_num;
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_num);

	if (proc_num == 0)
	{
		cout << "Start of application. Number of arguments: " << argc << endl;

		if (argc < 2) return 1;

		GraphLoader loader;

		/* LOAD GRAPH FROM FILE */
		Graph * originalGraph = loader.LoadGraph(argv[1]);
		cout << "Loaded Graph: NumberOfEdges = " << originalGraph->m_NumberOfEdgesOriginal << endl;
		originalGraph->Print();	
	
	

		/***************************************************/
		/************** FIND MAX BIGRAPH *******************/
		/***************************************************/

		cout << "Finding max bigraph" << endl;
		InitializeBestGraph(*originalGraph);

		// Try original graph
		if (m_BigraphMaker.MakeBigraph(*originalGraph))
		{
			cout << "ORIGINAL GRAPH IS RESULT" << endl;
			return 0;
		}

		#pragma omp parallel num_threads(NUMBER_OF_THREADS)
		{
			#pragma omp for schedule(static, m_FirstLvlGraphs.size() / NUMBER_OF_THREADS) 
			for (unsigned i = 0; i < m_FirstLvlGraphs.size(); i++)
			{
				Graph & tmp = m_FirstLvlGraphs[i];
				for (unsigned j = tmp.m_LastErasedEdge + 1; j < tmp.m_NumberOfEdgesOriginal; j++)
				{
					Graph newGraph(tmp);
					newGraph.RemoveEdge(j);

					#pragma omp critical
					m_Graphs.push_back(newGraph);
				}
			}

			//#pragma omp barrier
			#pragma omp for schedule(static,1)
			for (unsigned i = 0; i < m_Graphs.size(); i++)
			{
				FindMaxBigraphInternal(m_Graphs[i]);
			}
			
		}

		/* GET MAX BIPARTHITE GRAPH */
		cout << "RESULT Graph: NumberOfEdges = " << m_BestGraph->m_NumberOfEdgesCurrent << endl;
		m_BestGraph->Print();

		/* DELETE SECTION */
		DeleteGraphs(originalGraph, m_BestGraph);

	}
	else if(proc_num == 1)
	{

	}

	MPI_Finalize();
	return 0;
}