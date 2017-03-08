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
	m_ColoredNodes = queue<int>();
	m_ProcessedNodes.clear();
	m_Graph->m_NodeColors[0] = White;
	m_ColoredNodes.push(0);
}

bool BigraphMaker::ColorNodes()
{
	while(1)
	{
		int nodeIndex = GetNextColoredNode();
		Color neighbourColor = GetNeighbourColor(nodeIndex);
		
		//cout << "BigraphMaker::ColorNodes: Colorring neighbours of node " << nodeIndex << " (" << m_Graph->m_NodeColors[nodeIndex] << ")";
		//cout << " to color: " << neighbourColor << endl; 
		vector<int> neighbours = m_Graph->ColorNeighbourNodes(nodeIndex, neighbourColor);
		if (neighbours.size() == 0)
		{
			//cout << "BigraphMaker::ColorNodes: UNABLE TO COLOR GRAPH" << endl;
			return false;
		}

		AddNodeToProcessedNodes(nodeIndex);
		AddNodeNeighboursToQueue(neighbours);
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

void BigraphMaker::AddNodeNeighboursToQueue(vector<int> & neighbours)
{
	for (unsigned i = 0; i < neighbours.size(); i++)
	{
		if (m_ProcessedNodes.find(neighbours[i]) == m_ProcessedNodes.end())
		{
			//cout << "BigraphMaker::ColorNodes: Pushing not processed node: " << i << " to nodeToColor queue" << endl;
			m_ColoredNodes.push(neighbours[i]);
		}		
	}
}

void BigraphMaker::AddNodeToProcessedNodes(int nodeIndex)
{
	//cout << "BigraphMaker::AddNodeToProcessedNodes: Adding node " << nodeIndex << " to processed nodes" << endl;
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
	//cout << "BigraphMaker::ColorNodes: Nodes to color queue is empty. Index of first not yet colored node: " << notColoredNode << endl;
	if (notColoredNode > -1)
	{
		m_Graph->m_NodeColors[notColoredNode] = White;
		m_ColoredNodes.push(notColoredNode);
	}
}