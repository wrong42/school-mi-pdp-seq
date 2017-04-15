#include <iostream>
#include <mpi.h>
#include "maxBigraphSolver.h"
#include "graphLoader.h"
#include "graph.h"
#include "edge.h"

using namespace std;

void DeleteGraphs(Graph * loadedGraph, Graph * resultGraph);

int main(int argc, char * argv[])
{
	MPI_Init(&argc, &argv);
	cout << "Start of application. Number of arguments: " << argc << endl;

	if (argc < 2) return 1;

	GraphLoader loader;
	MaxBigraphSolver solver;

	/* LOAD GRAPH FROM FILE */
	Graph * graph = loader.LoadGraph(argv[1]);
	cout << "Loaded Graph: NumberOfEdges = " << graph->m_NumberOfEdgesOriginal << endl;
	graph->Print();

	/* GET MAX BIPARTHITE GRAPH */
	Graph * result = solver.FindMaxBigraph(*graph);
	cout << "RESULT Graph: NumberOfEdges = " << result->m_NumberOfEdgesCurrent << endl;
	result->Print();

	/* DELETE SECTION */
	DeleteGraphs(graph, result);

	MPI_Finalize();
	return 0;
}

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