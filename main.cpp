#include <iostream>
#include "maxBigraphSolver.h"
#include "graphLoader.h"
#include "graph.h"
#include "edge.h"

using namespace std;

int main(int args, char * argv[])
{
	cout << "Start of application. Number of arguments: " << args << endl;

	if (args < 2) return 1;

	GraphLoader loader;

	Graph * graph = loader.LoadGraph(argv[1]);
	cout << "Graph loaded: Number of edges = " << graph->m_Edges.size() << endl;
	graph->Print();
//	MaxBigraphSolver solver;
//	Graph * result = solver.FindMaxBigraph(*graph);
//	cout << "RESULT: NumberOfEdges = " << result->m_Edges.size() << endl;

	for (int i = 0; i < graph->m_Edges.size(); i++)
	{
		delete graph->m_Edges[i];
	}

	graph->m_Edges.clear();
	delete graph;
	return 0;
}
