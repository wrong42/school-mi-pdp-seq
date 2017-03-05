#include <iostream>
#include <fstream>
#include "inputLoader.h"

using namespace std;

Graph * GraphLoader::LoadGraph(const char * file)
{
	if (!FileExists(file))
	{
		cout << "File: " << file << " does NOT exists or is unaccessible." << endl;
		return new Graph(0);
	}

	ifstream ifs(file)
	unsigned numberOfNodes, tmp;

	ifs >> numberOfNodes;
	Graph * graph = new Graph(numberOfNodes);

	for (int i = 0; i < 
}

bool GraphLoader::FileExists(const char * file)
{
	ifstream ifs(file)
	return ifs.good();
}
