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
}

Graph::Graph(const Graph & src)
{
	m_NumberOfNodes = src.m_NumberOfNodes;
	m_AdjMatrixSize = src.m_AdjMatrixSize;
	m_AdjMatrix = new bool[m_AdjMatrixSize];
	m_NodeColors = new Color[m_NumberOfNodes];
	
	memcpy((void*)m_AdjMatrix, src.m_AdjMatrix, m_AdjMatrixSize * sizeof(bool));
	memset((void*)m_NodeColors, 0, m_NumberOfNodes * sizeof(Color));
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
