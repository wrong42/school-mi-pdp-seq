#include <iostream>
#include "bigraphMaker.h"

using namespace std;

bool BigraphMaker::MakeBigraph(Graph & graph)
{
	m_Graph = & graph;
	
	InitializeQueue();

	return ColorNodes();
}

/* Initialize queue with nodes to color. First node colored White */
void BigraphMaker::InitializeQueue()
{
	m_Graph->m_NodeColors[0] = White;
	m_ColoredNodes.push(0);
}

bool BigraphMaker::ColorNodes()
{
	while(1)
	{
		int nodeIndex = GetNextColoredNode();
		Color neighbourColor = GetNeighbourColor(nodeIndex);
		
		//cout << "MaxBigraphSolver::TryMakeBigraph: Colorring neighbours of node " << nodeIndex << " (" << m_Graph->m_NodeColors[nodeIndex] << ")";
		//cout << " to color: " << neighbourColor << endl; 
		if (!m_Graph->ColorNeighbourNodes(nodeIndex, neighbourColor))
		{
			//cout << "MaxBigraphSolver::TryMakeBigraph: UNABLE TO COLOR GRAPH" << endl;
			return false;
		}

		AddNodeToProcessedNodes(nodeIndex);
		AddNodeNeighboursToQueue(nodeIndex);
		AddNotColoredNodeIfAny();

		if (m_ColoredNodes.empty())
		{
			break;
		}
	}

	return true;
}

int BigraphMaker::GetNextColoredNode()
{
	int nodeIndex = m_ColoredNodes.front();
	m_ColoredNodes.pop();
	return nodeIndex;
}

Color BigraphMaker::GetNeighbourColor(int nodeIndex) const
{
	return m_Graph->m_NodeColors[nodeIndex] == Black ? White : Black;
}

void BigraphMaker::AddNodeNeighboursToQueue(int nodeIndex)
{
	for (int i = 0; i < m_Graph->m_NumberOfNodes; i++)
	{
		if (m_Graph->AreNeighbours(nodeIndex, i))
		{
			if (m_ProcessedNodes.find(i) == m_ProcessedNodes.end())
			{
				//cout << "MaxBigraphSolver::TryMakeBigraph: Pushing not processed node: " << i << " to nodeToColor queue" << endl;
				m_ColoredNodes.push(i);
			}
		}
	}
}

void BigraphMaker::AddNodeToProcessedNodes(int nodeIndex)
{
	m_ProcessedNodes.insert(nodeIndex);
}

/* 
* If the que with colored nodes is empty, and there is still at least one
* not colored node, the not colored node is colored and added to the queue
*/
void BigraphMaker::AddNotColoredNodeIfAny()
{
	if (!m_ColoredNodes.empty())
	{
		return;
	}

	int notColoredNode = m_Graph->GetFirstUncoloredNode(); 
	//cout << "MaxBigraphSolver::TryMakeBigraph: Nodes to color queue is empty. Index of first not yet colored node: " << notColoredNode << endl;
	if (notColoredNode > -1)
	{
		m_Graph->m_NodeColors[notColoredNode] = White;
		m_ColoredNodes.push(notColoredNode);
	}
}