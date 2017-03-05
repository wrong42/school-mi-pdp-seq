#include <iostream>
#include <cstring>
#include "graph.h"

using namespace std;

Graph::Graph(int numberOfNodes)
{
	m_NumberOfNodes = numberOfNodes;
	m_AdjMatrixSize = GetSizeOfAdjMatrix();
	m_AdjMatrix = new bool[m_AdjMatrixSize];
	m_NodeColors = new Color[numberOfNodes];
	
	memset((void*)m_NodeColors, Undefined, numberOfNodes);
}

Graph::Graph(const Graph & src)
{
	m_NumberOfNodes = src.m_NumberOfNodes;
	m_Edges = src.m_Edges;
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
	if (node1 == node2)
	{
		return false;
	}

	int edgeIndex = GetEdgeIndex(node1, node2);
	return m_AdjMatrix[edgeIndex];
}

int Graph::GetEdgeIndex(int node1, int node2) const
{
	if (node1 > node2)
	{
		int tmp = node1;
		node1 = node2;
		node2 = tmp;
	}

	int sum = (m_NumberOfNodes - node1 - 1) * (m_NumberOfNodes - node1) / 2;
	int baseIndex = m_AdjMatrixSize - sum;
	int edgeIndex = baseIndex + (node2 - node1);

	return edgeIndex;
}

void Graph::RemoveOneEdge()
{
	Edge * edge = m_Edges.back();
	int edgeIndex = GetEdgeIndex(edge->Node1, edge->Node2);
	m_AdjMatrix[edgeIndex] = false;
	m_Edges.pop_back();
}

void Graph::RemoveEdge(unsigned index)
{
	Edge * edge = m_Edges.at(index);
	int edgeIndex = GetEdgeIndex(edge->Node1, edge->Node2);
	m_AdjMatrix[edgeIndex] = false;
	
	vector<Edge*>::iterator it = m_Edges.begin();
	for (int i = 0; i < index; i++)
		++it;
	
	m_Edges.erase(it);
}

void Graph::Print() const
{
	int index = 0;
	
	cout << "Graph::Print: " << endl;
	for (int i = 0; i < m_NumberOfNodes; i++)
	{
		for (int j = i; j < m_NumberOfNodes; j++)
		{
			cout << m_AdjMatrix[index++] << " ";
		}
		
		cout << endl;
	}
	
}
