#ifndef __GRAPH_H__2222
#define __GRAPH_H__2222

#include <vector>
#include <set>
#include <map>
#include "edge.h"
#include "colors.h"

class Graph
{
	public:
		int m_NumberOfNodes;
		bool * m_AdjMatrix;
		Color * m_NodeColors;
		std::vector<Edge*> m_Edges;
		std::set<int> m_MissingEdgesById;
		std::map<std::pair<int,int>, int> m_EdgeMappings;

	public:
		Graph(int numberOfNodes);
		Graph(const Graph &);
	  ~Graph();

		void RemoveEdge(unsigned);
		int GetEdgeIndex(int, int) const;
		bool AreNeighbours(int, int) const; 		
		void Print() const;
		std::vector<int> ColorNeighbourNodes(int, Color);
		int GetFirstUncoloredNode() const;
	private:
		int GetSizeOfAdjMatrix() const;	
	private:
		int m_AdjMatrixSize;

};

#endif // __GRAPH_H__2222
