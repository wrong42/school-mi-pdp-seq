#include <iostream>
#include <cstring>
#include <queue>
#include "graph.h"

using namespace std;

Graph::Graph(int numberOfNodes)
{
	m_NumberOfNodes = numberOfNodes;
	m_LastErasedEdge = 0;
	m_AdjMatrixSize = GetSizeOfAdjMatrix();
	m_AdjMatrix = new bool[m_AdjMatrixSize];
	m_NodeColors = new Color[numberOfNodes];

	memset((void*)m_NodeColors, Undefined, numberOfNodes * sizeof(Color));
}

Graph::Graph(const Graph & src)
{
	m_NumberOfNodes = src.m_NumberOfNodes;
	m_NumberOfEdgesOriginal = src.m_NumberOfEdgesOriginal;
	m_NumberOfEdgesCurrent = src.m_NumberOfEdgesCurrent;
	m_AdjMatrixSize = src.m_AdjMatrixSize;
	m_Edges = src.m_Edges;

	m_AdjMatrix = new bool[m_AdjMatrixSize];
	m_EdgeMatrix = new bool[m_NumberOfEdgesOriginal];
	m_NodeColors = new Color[m_NumberOfNodes];
	
	memcpy((void*)m_AdjMatrix, src.m_AdjMatrix, m_AdjMatrixSize * sizeof(bool));
	memcpy((void*)m_EdgeMatrix, src.m_EdgeMatrix, m_NumberOfEdgesOriginal * sizeof(bool));
	memset((void*)m_NodeColors, Undefined, m_NumberOfNodes * sizeof(Color));
}

Graph::~Graph()
{
	delete[] m_AdjMatrix;
	delete[] m_EdgeMatrix;
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

	int edgeIndex = GetEdgeIndex(node1, node2);
	//cout << "Graph::AreNeighbours: AreNeighbours=" << m_AdjMatrix[edgeIndex] << endl;
	return m_AdjMatrix[edgeIndex];
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

void Graph::RemoveNextEdge()
{
	m_EdgeMatrix[m_LastErasedEdge] = false;
	Edge * e = m_Edges[m_LastErasedEdge];
	int adjMatrixIndex = GetEdgeIndex(e->Node1, e->Node2);
	m_AdjMatrix[adjMatrixIndex];

	++m_LastErasedEdge;
	--m_NumberOfEdgesCurrent;
}

void Graph::RemoveEdge(unsigned index)
{
	m_EdgeMatrix[index] = false;
	Edge * e = m_Edges[index];
	int adjMatrixIndex = GetEdgeIndex(e->Node1, e->Node2);
	m_AdjMatrix[adjMatrixIndex];

	m_LastErasedEdge = index;
	--m_NumberOfEdgesCurrent;
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

bool Graph::ColorNeighbourNodes(int nodeIndex, Color color)
{
	for (int i = 0; i < m_NumberOfNodes; i++)
	{
		if (AreNeighbours(nodeIndex, i))
		{
			if (m_NodeColors[i] != Undefined && m_NodeColors[i] != color)
			{
				//cout << "Graph::ColorNeighbourNodes: Unable to color node " << i << ". Node is already colored: " << m_NodeColors[i] << endl;
				return false;
			}
			else
			{
				//cout << "Graph::ColorNeighbourNodes: Coloring node " << i << " to color " << color << endl;
				m_NodeColors[i] = color;
			}
		}
	}

	return true;
}
