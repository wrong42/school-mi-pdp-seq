#ifndef __GRAPH_H__2222
#define __GRAPH_H__2222

#include <list>
#include "edge.h"
#include "colors.h"

class Graph
{
	public:
		int m_NumberOfNodes;
		bool * m_AdjMatrix;
		std::list<Edge*> m_Edges;
		Color * m_NodeColors;

	public:
		Graph(int numberOfNodes);
		Graph(const Graph &);
	  ~Graph();

		void RemoveOneEdge();
		bool AreNeighbours(int, int) const; 		
		int GetEdgeIndex(int, int) const;
	private:
		int GetSizeOfAdjMatrix() const;
	
	private:
		int m_AdjMatrixSize;
};

#endif // __GRAPH_H__2222
