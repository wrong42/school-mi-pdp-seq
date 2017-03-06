#ifndef __GRAPH_LOADER_H__
#define __GRAPH_LOADER_H__

#include <string>
#include <fstream>
#include "graph.h"

class GraphLoader
{
	public:
		static Graph * LoadGraph(const char*);
	private:
		static bool FileExists(const char*);
		static int GetNumberOfNodes(const std::string &);
	private:
		static std::ifstream & inputFile;
};

#endif // __GRAPH_LOADER_H__
