#ifndef __MAX_BIGRAPH_SOLVER_H__
#define __MAX_BIGRAPH_SOLVER_H__

#include "graph.h"
#include <stack>

class MaxBigraphSolver
{
	public:
				MaxBigraphSolver();
				~MaxBigraphSolver();
		Graph * FindMaxBigraph(Graph &);
	private:
		void FindMaxBigraphInternal(Graph &);
		bool TryMakeBigraph(Graph &) const;
		bool TryColorNode(Graph &, int) const;
	private:
		Graph * m_BestGraph;
		Graph * m_OriginalGraph;

		std::stack<Graph*> m_GraphStack;
};

#endif // __MAX_BIGRAPH_SOLVER_H__
