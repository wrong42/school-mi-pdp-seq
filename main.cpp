#include <iostream>
#include "maxBigraphSolver.h"
#include "graphLoader.h"
#include "graph.h"
#include "edge.h"

using namespace std;

void DeleteGraphs(Graph * loadedGraph, Graph * resultGraph);

int main(int args, char * argv[])
{
	cout << "Start of application. Number of arguments: " << args << endl;

	if (args < 2) return 1;

	GraphLoader loader;
	MaxBigraphSolver solver;

	/* LOAD GRAPH FROM FILE */
	Graph * graph = loader.LoadGraph(argv[1]);
	cout << "Loaded Graph: NumberOfEdges = " << graph->m_Edges.size() << endl;
	graph->Print();

	/* GET MAX BIPARTHITE GRAPH */
	Graph * result = solver.FindMaxBigraph(*graph);
	cout << "RESULT Graph: NumberOfEdges = " << result->m_Edges.size() << endl;
	result->Print();

	/* DELETE SECTION */
	DeleteGraphs(graph, result);

	return 0;
}

void DeleteGraphs(Graph * loadedGraph, Graph * resultGraph)
{
	if (resultGraph != loadedGraph)
	{
		delete resultGraph;
	}
	
	for (unsigned i = 0; i < loadedGraph->m_Edges.size(); i++)
	{
		delete loadedGraph->m_Edges[i];
	}
	
	loadedGraph->m_Edges.clear();
	delete loadedGraph;
}