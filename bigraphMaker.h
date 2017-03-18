#ifndef __BIGRAPH_MAKER_H__
#define __BIGRAPH_MAKER_H__

#include <set>
#include <queue>
#include <vector>
#include "graph.h"

class BigraphMaker
{
	public:
		bool MakeBigraph(Graph &);
		std::queue<int> m_ColoredNodes;
		std::set<int> m_ProcessedNodes;

	private:
		void InitializeQueue();
		bool ColorNodes();
		int GetNextColoredNode();
		void AddNodeToProcessedNodes(int);
		void AddNodeNeighboursToQueue(std::vector<int> &);
		void AddNotColoredNodeIfAny();
		Color GetNeighbourColor(int) const;
	private:
		Graph * m_Graph;

};

#endif // __BIGRAPH_MAKER_H__