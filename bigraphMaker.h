#ifndef __BIGRAPH_MAKER_H__
#define __BIGRAPH_MAKER_H__

#include <set>
#include <queue>
#include "graph.h"

class BigraphMaker
{
	public:
		bool MakeBigraph(Graph &) const;
	private:
		queue<int> nodesToColor;
		set<int> processedNodes;
};

#endif // __BIGRAPH_MAKER_H__