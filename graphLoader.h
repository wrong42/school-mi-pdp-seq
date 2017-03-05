#ifndef __GRAPH_LOADER_H__
#define __GRAPH_LOADER_H__

#include "graph.h"

class GraphLoader
{
	public:
		static Graph * LoadGraph(const char*);
	private:
		static bool FileExists(const char*);
};

#endif // __GRAPH_LOADER_H__
