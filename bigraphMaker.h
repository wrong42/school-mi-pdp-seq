#ifndef __BIGRAPH_MAKER_H__
#define __BIGRAPH_MAKER_H__

#include <set>
#include <queue>
#include "graph.h"

class BigraphMaker
{
	public:
		bool MakeBigraph(Graph &);

	private:
		void InitializeQueue();
		bool ColorNodes();
		int GetNextColoredNode();
		void AddNodeToProcessedNodes(int);
		void AddNodeNeighboursToQueue(int);
		void AddNotColoredNodeIfAny();
		Color GetNeighbourColor(int) const;
	private:
		Graph * m_Graph;

		std::queue<int> m_ColoredNodes;
		std::set<int> m_ProcessedNodes;
};

#endif // __BIGRAPH_MAKER_H__