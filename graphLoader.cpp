#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include "graphLoader.h"

using namespace std;

GraphLoader::~GraphLoader()
{
	delete m_InputFile;
}

Graph * GraphLoader::LoadGraph(const char * file)
{
	if (!FileExists(file))
	{
		cout << "File: " << file << " does NOT exists or is unaccessible." << endl;
		return new Graph(0);
	}

	OpenInputFile(file);
	GetNumberOfNodes();
	CreateGraph();
	LoadGraphInternal();
	CloseInputFile();

	return m_Graph; 
}

void GraphLoader::LoadGraphInternal()
{
	string line;
	int index = 0, edgeCounter = 0;

	for (int i = 0; i < m_NumberOfGraphNodes; i++)
	{
		std::getline(*m_InputFile, line);

		for (int j = 0; j < m_NumberOfGraphNodes; j++)
		{
			if (j > i)
			{
				if (line[j] == '1')
				{
					m_Graph->m_AdjMatrix[index++] = true;
					m_Edges.push_back(new Edge(edgeCounter++, i, j));
				}
				else
				{
					m_Graph->m_AdjMatrix[index++] = false;
				}
				//cout << "GraphLoader::LoadGraph: i=" << i << " j=" << j << " readValue=" << line[j] << endl;
				//InsertNewEdge(index, line[j], new Edge(edgeId++, i, j));
			}
		}

	}

	bool * edgeMatrix = new bool[edgeCounter];
	memset((void*)edgeMatrix, 1, edgeCounter * sizeof(bool));
	m_Graph->m_EdgeMatrix = edgeMatrix;
	m_Graph->m_NumberOfEdgesOriginal = edgeCounter;
	m_Graph->m_NumberOfEdgesCurrent = edgeCounter;


	Edge ** edges = new Edge*[edgeCounter];
	for (unsigned u = 0; u < m_Edges.size(); u++)
	{
		edges[u] = m_Edges[u];
	}

	m_Graph->m_Edges = edges;
}

void GraphLoader::InsertNewEdge(int & index, char value, Edge * edge)
{
	switch (value)
	{
		case '0':
			m_Graph->m_AdjMatrix[index++] = false;
			delete edge;
			break;
		case '1':
			m_Graph->m_AdjMatrix[index++] = true;
			//m_Edges.push_back(edge);
			break;
		case '\n':
			delete edge;
			break;
		default:
			cout << "ERROR> GraphLoader::LoadGraph: Invalid character in AdjMatrix: " << value << endl;
			delete edge;
			delete m_Graph;
			throw std::invalid_argument( "Invalid character in AdjMatrix" );
	}
}

void GraphLoader::CreateGraph()
{
	m_Graph = new Graph(m_NumberOfGraphNodes);
}

void GraphLoader::OpenInputFile(const char * file)
{
	m_InputFile = new ifstream(file);
	cout << "Input file reader created. Reader state: " << m_InputFile->good() << endl;
}

void GraphLoader::CloseInputFile()
{
	m_InputFile->close();
}

void GraphLoader::GetNumberOfNodes()
{
	string str;

	std::getline(*m_InputFile, str);
	istringstream iss(str);
	iss >> m_NumberOfGraphNodes;
}

bool GraphLoader::FileExists(const char * file) const
{
	ifstream ifs(file);
	return ifs.good();
}
