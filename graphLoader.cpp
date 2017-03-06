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

	string line;
	int numberOfNodes, index = 0;
	
	ifstream infile(file);
	std::getline(infile, line);
	numberOfNodes = GetNumberOfNodes(line);
	Graph * graph = new Graph(numberOfNodes);

	for (int i = 0; i < numberOfNodes; i++)
	{
		std::getline(infile, line);

		for (int j = 0; j < numberOfNodes; j++)
		{
			if (j > i)
			{
				cout << "GraphLoader::LoadGraph: i=" << i << " j=" << j << " readValue=" << line[j] << endl;
				switch (line[j])
				{
					case '0':
						graph->m_AdjMatrix[index++] = false;
						graph->m_Edges.push_back(new Edge(i,j));
						break;
					case '1':
						graph->m_AdjMatrix[index++] = true;
						graph->m_Edges.push_back(new Edge(i,j));
						break;
					case '\n':
						break;
					default:
						cout << "ERROR> GraphLoader::LoadGraph: Invalid character in AdjMatrix: " << line[j] << endl;
						return new Graph(0);
				}				
			}
		}

	}

	infile.close();	
	return graph; 
}

int GraphLoader::GetNumberOfNodes(const string & line)
{
	int numberOfNodes;
	istringstream iss(line);
	iss >> numberOfNodes;
	return numberOfNodes;
}

bool GraphLoader::FileExists(const char * file)
{
	ifstream ifs(file);
	return ifs.good();
}
