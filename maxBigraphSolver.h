#ifndef __MAX_BIGRAPH_SOLVER_H__
#define __MAX_BIGRAPH_SOLVER_H__

#include "graph.h"
#include <set>
#include <stack>
#include <vector>

class MaxBigraphSolver
{
	public:
				MaxBigraphSolver();
				~MaxBigraphSolver();
		Graph * FindMaxBigraph(Graph &);
	private:
		bool TryMakeBigraph(Graph &) const;
		bool TryColorNode(Graph &, int) const;
		bool GraphHasBeenProcessed(const Graph &) const;
	private:
		Graph * m_BestGraph;
		Graph * m_OriginalGraph;

		std::stack<Graph*> m_GraphStack;
		std::set<std::set<int> > m_ProcessedGraphsByEdge;
		std::vector<std::set<int> *> m_MissingEdges;
};

#endif // __MAX_BIGRAPH_SOLVER_H__
