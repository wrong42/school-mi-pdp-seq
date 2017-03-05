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
		Color * m_Colors;

	public:
		Graph(int numberOfNodes);
		Graph(const Graph &);
	  ~Graph(); 		
};

#endif // __GRAPH_H__2222
