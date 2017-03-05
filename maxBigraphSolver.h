#ifndef __MAX_BIGRAPH_SOLVER_H__
#define __MAX_BIGRAPH_SOLVER_H__

#include "graph.h"

class MaxBigraphSolver
{
	public:
		Graph * FindMaxBigraph(Graph &);
	private:
		void FindMaxBigraphInternal(Graph &);
		bool TryMakeBigraph(Graph &) const;
		bool TryColorNode(Graph &, int) const;
	private:
		Graph * bestState;
};

#endif // __MAX_BIGRAPH_SOLVER_H__
