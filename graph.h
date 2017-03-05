#ifndef __GRAPH_H__2222
#define __GRAPH_H__2222

#include <vector>
#include "edge.h"
#include "colors.h"

class Graph
{
	public:
		int m_NumberOfNodes;
		bool * m_AdjMatrix;
		std::vector<Edge*> m_Edges;
		Color * m_NodeColors;

	public:
		Graph(int numberOfNodes);
		Graph(const Graph &);
	  ~Graph();

		void RemoveOneEdge();
		void RemoveEdge(unsigned);
		bool AreNeighbours(int, int) const; 		
		int GetEdgeIndex(int, int) const;
	private:
		int GetSizeOfAdjMatrix() const;
	
	private:
		int m_AdjMatrixSize;
};

#endif // __GRAPH_H__2222
