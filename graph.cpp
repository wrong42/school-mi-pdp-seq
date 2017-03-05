#include <iostream>
#include <cstring>
#include "graph.h"

using namespace std;

Graph::Graph(int numberOfNodes)
{
	m_NumberOfNodes = numberOfNodes;
	int matrixSize = GetSizeOfAdjMatrix();
	m_AdjMatrix = new bool[matrixSize];
	m_NodeColors = new Color[numberOfNodes];
}

Graph::Graph(const Graph & src)
{
	m_NumberOfNodes = src.m_NumberOfNodes;
	int matrixSize = GetSizeOfAdjMatrix();
	m_AdjMatrix = new bool[matrixSize];
	m_NodeColors = new Color[m_NumberOfNodes];
	
	memcpy((void*)m_AdjMatrix, src.m_AdjMatrix, matrixSize * sizeof(bool));
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
