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
		std::vector<Graph> m_Graphs;
		BigraphMaker m_BigraphMaker;
};

#endif // __MAX_BIGRAPH_SOLVER_H__
