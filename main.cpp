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
	graph->Print();
	cout << "Loaded Graph: NumberOfEdges = " << graph->m_Edges.size() << endl;
	MaxBigraphSolver solver;
	Graph * result = solver.FindMaxBigraph(*graph);
	cout << "RESULT Graph: NumberOfEdges = " << result->m_Edges.size() << endl;
	result->Print();

	for (unsigned i = 0; i < graph->m_Edges.size(); i++)
	{
		delete graph->m_Edges[i];
	}

	graph->m_Edges.clear();
	delete graph;
	result->m_Edges.clear();
	delete result;

	return 0;
}
