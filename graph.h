#ifndef __GRAPH_H__2222
#define __GRAPH_H__2222

#include <vector>
#include <set>
#include "edge.h"
#include "colors.h"

class Graph
{
	public:
		int m_NumberOfNodes;
		int m_NumberOfEdgesOriginal;
		int m_NumberOfEdgesCurrent;
		int m_LastErasedEdge;

		bool * m_AdjMatrix;
		bool * m_EdgeMatrix;
		Color * m_NodeColors;
		Edge ** m_Edges; // NEW - shared for all graphs

		//std::set<int> m_MissingEdgesById;
		//std::vector<Edge*> m_Edges; //OLD

	public:
		Graph(int numberOfNodes);
		Graph(const Graph &);
	  ~Graph();

		void RemoveNextEdge();
		void RemoveEdge(unsigned);
		int GetEdgeIndex(int, int) const;
		bool AreNeighbours(int, int) const; 		
		void Print() const;
		bool ColorNeighbourNodes(int, Color);
		int GetFirstUncoloredNode() const;
	private:
		int GetSizeOfAdjMatrix() const;
	
	private:
		int m_AdjMatrixSize;

};

#endif // __GRAPH_H__2222
