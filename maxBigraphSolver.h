#ifndef __MAX_BIGRAPH_SOLVER_H__
#define __MAX_BIGRAPH_SOLVER_H__

#include <set>
#include <stack>
#include <queue>
#include <vector>
#include "graph.h"
#include "bigraphMaker.h"

class MaxBigraphSolver
{
	public:
				MaxBigraphSolver();
				~MaxBigraphSolver();
		Graph * FindMaxBigraph(Graph &);
	private:
		void FindMaxBigraphInternal(Graph);
		void AddProcessedGraph(Graph *);
		void AcceptBetterGraph(Graph *);

	private:
		Graph * m_BestGraph;
		Graph * m_OriginalGraph;
		BigraphMaker m_BigraphMaker;
		std::stack<Graph*> m_GraphStack;
};

#endif // __MAX_BIGRAPH_SOLVER_H__
