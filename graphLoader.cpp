#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "graphLoader.h"

using namespace std;

Graph * GraphLoader::LoadGraph(const char * file)
{
	if (!FileExists(file))
	{
		cout << "File: " << file << " does NOT exists or is unaccessible." << endl;
		return new Graph(0);
	}

	ifstream infile(file);
	int numberOfNodes, tmp;
	
	string line;
	std::getline(infile, line)
	istringstream iss(line);
	iss >> numberOfNodes;
	
	for (int i = 0; i < numberOfNodes; i++)
	{
		std::getline(infile, line)
		while();
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
	
	while (std::getline(file, line))
	{
		istringstream iss(line);
		int a, b;
		if (!(iss >> a >> b)) { break; } // error

		// process pair (a,b)
		
			

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
	}
	
	return graph; 
}

bool GraphLoader::FileExists(const char * file)
{
	ifstream ifs(file);
	return ifs.good();
}
