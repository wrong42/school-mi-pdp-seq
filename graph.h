#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <list>
#include "edge.h"
#include "color.h"

class Graph
{
	public:
		int m_NumberOfNodes;
		bool * m_AdjMatrix;
		list<Edge*> m_Edges;
		Color * m_Colors;

	public:
		Graph(int numberOfNodes);
		Graph(const Graph &);
	  ~Graph(); 		
};

#endif // __GRAPH_H__
