#include <iostream>
#include <cstring>
#include <queue>
#include <list>
#include "graph.h"

using namespace std;

Graph::Graph(int numberOfNodes)
{
	m_NumberOfNodes = numberOfNodes;
	m_AdjMatrixSize = GetSizeOfAdjMatrix();
	m_AdjMatrix = new bool[m_AdjMatrixSize];
	m_NodeColors = new Color[numberOfNodes];

	memset((void*)m_NodeColors, Undefined, numberOfNodes * sizeof(Color));
}

Graph::Graph(const Graph & src) : m_Edges(src.m_Edges), m_MissingEdgesById(src.m_MissingEdgesById), m_EdgeMappings(src.m_EdgeMappings)
{
	m_NumberOfNodes = src.m_NumberOfNodes;
	//m_Edges = src.m_Edges;
	m_AdjMatrixSize = src.m_AdjMatrixSize;
	m_AdjMatrix = new bool[m_AdjMatrixSize];
	m_NodeColors = new Color[m_NumberOfNodes];
	
	memcpy((void*)m_AdjMatrix, src.m_AdjMatrix, m_AdjMatrixSize * sizeof(bool));
	memset((void*)m_NodeColors, Undefined, m_NumberOfNodes * sizeof(Color));
}

Graph::~Graph()
{
	delete[] m_AdjMatrix;
	delete[] m_NodeColors;
}

int Graph::GetSizeOfAdjMatrix() const
{
	int numberOfRows = m_NumberOfNodes - 1;
	int arithSum = numberOfRows * m_NumberOfNodes / 2;
	return arithSum;
}

bool Graph::AreNeighbours(int node1, int node2) const
{
	//cout << "Graph::AreNeighbours: Nodes: node1=" << node1 << ", node2=" << node2 << endl;
	if (node1 == node2)
	{
		return false;
	}

	//int edgeIndex = GetEdgeIndex(node1, node2);
	//cout << "Graph::AreNeighbours: AreNeighbours=" << m_AdjMatrix[edgeIndex] << endl;
	map<pair<int,int>, int>::const_iterator pos = m_EdgeMappings.find(pair<int,int>(node1, node2));
	if (pos == m_EdgeMappings.end())
		return false;

	return m_AdjMatrix[pos->second];
}

int Graph::GetEdgeIndex(int node1, int node2) const
{
	//cout << "Graph::GetEdgeIndex: Nodes: node1=" << node1 << ", node2=" << node2 << endl;

	if (node1 > node2)
	{
		int tmp = node1;
		node1 = node2;
		node2 = tmp;
	}

	int sum = (m_NumberOfNodes - node1 - 1) * (m_NumberOfNodes - node1) / 2;
	int baseIndex = m_AdjMatrixSize - sum;
	int edgeIndex = baseIndex + (node2 - node1 - 1);

	//cout << "Graph::GetEdgeIndex: Edge index = " << edgeIndex << endl;
	return edgeIndex;
}

void Graph::RemoveEdge(unsigned index)
{
	//cout << "Graph::RemoveEdge: Removing edge with index " << index << endl;
	Edge * edge = m_Edges.at(index);
	int edgeIndex = GetEdgeIndex(edge->Node1, edge->Node2);
	m_AdjMatrix[edgeIndex] = false;
	
	vector<Edge*>::iterator it = m_Edges.begin();
	for (unsigned i = 0; i < index; i++)
		++it;
	
	m_Edges.erase(it);
	m_MissingEdgesById.insert(index);
}

void Graph::Print() const
{
	int index = 0;
	
	cout << "Graph::Print: " << endl;
	for (int i = 0; i < m_NumberOfNodes; i++)
	{
		for (int j = i; j < m_NumberOfNodes - 1; j++)
		{
			cout << m_AdjMatrix[index++] << " ";
		}
		
		cout << endl;
	}

	cout << "\nNodeColors: " << endl;
	for (int i = 0; i < m_NumberOfNodes; i++)
	{
		cout << m_NodeColors[i] << " ";
	}

	cout << endl;
}

int Graph::GetFirstUncoloredNode() const
{
	for (int i = 0; i < m_NumberOfNodes; i++)
	{
		if (m_NodeColors[i] == Undefined)
		{
			return i;
		}
	}

	return -1;
}

vector<int> Graph::ColorNeighbourNodes(int nodeIndex, Color color)
{
	vector<int> neighbours;
	for (int i = 0; i < m_NumberOfNodes; i++)
	{
		if (AreNeighbours(nodeIndex, i))
		{
			if (m_NodeColors[i] != Undefined && m_NodeColors[i] != color)
			{
				//cout << "Graph::ColorNeighbourNodes: Unable to color node " << i << ". Node is already colored: " << m_NodeColors[i] << endl;
				vector<int> empty;
				return empty;
			}
			else
			{
				//cout << "Graph::ColorNeighbourNodes: Coloring node " << i << " to color " << color << endl;
				m_NodeColors[i] = color;
				neighbours.push_back(i);
			}
		}
	}

	//cout << "GRAPH::Color neighbours - count = " << neighbours.size() << endl;
	return neighbours;
}
