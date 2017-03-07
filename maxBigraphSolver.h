#ifndef __MAX_BIGRAPH_SOLVER_H__
#define __MAX_BIGRAPH_SOLVER_H__

#include <set>
#include <stack>
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
		bool TryMakeBigraph(Graph &);
		bool GraphHasBeenProcessed(const Graph &) const;
		bool PossiblyBetterGraph(const Graph &) const;
		void TryPossiblyBetterGraph(Graph *);
		void AddChildGraphsToStack(Graph *);
		void AddProcessedGraph(Graph *);
		void AcceptBetterGraph(Graph *);
	private:
		Graph * m_BestGraph;
		Graph * m_OriginalGraph;

		BigraphMaker m_BigraphMaker;

		std::stack<Graph*> m_GraphStack;
		std::set<std::set<int> > m_ProcessedGraphsByEdge;
		std::vector<std::set<int> *> m_MissingEdges;

		static const int MaximumNumberOfMissingEdges = 4;
};

#endif // __MAX_BIGRAPH_SOLVER_H__
