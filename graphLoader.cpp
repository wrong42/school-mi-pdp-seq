#include <iostream>
#include <fstream>
#include "graphLoader.h"

using namespace std;

Graph * GraphLoader::LoadGraph(const char * file)
{
	if (!FileExists(file))
	{
		cout << "File: " << file << " does NOT exists or is unaccessible." << endl;
		return new Graph(0);
	}

	ifstream ifs(file);
	unsigned numberOfNodes, tmp, index = 0;

	ifs >> numberOfNodes;
	Graph * graph = new Graph(numberOfNodes);


	for (int i = 0; i < numberOfNodes; i++)
	{
		for (int j = 0; j < numberOfNodes; j++)
		{
			ifs >> tmp;

			if (j > i)
			{
				cout << "GraphLoader::LoadGraph: i=" << i << " j=" << j << " readValue=" << tmp << endl;
				graph->m_AdjMatrix[index++] = tmp == 0 ? false : true;
				graph->m_Edges.push_back(new Edge(i,j));
			}
		}
	}

	ifs.close();
	return graph; 
}

bool GraphLoader::FileExists(const char * file)
{
	ifstream ifs(file);
	return ifs.good();
}
