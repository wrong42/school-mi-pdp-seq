#ifndef __GRAPH_LOADER_H__
#define __GRAPH_LOADER_H__

#include <string>
#include <fstream>
#include "graph.h"

class GraphLoader
{
	public:
		~GraphLoader();
		Graph * LoadGraph(const char*);
	private:
		bool FileExists(const char*) const;
		void GetNumberOfNodes();
		void OpenInputFile(const char*);
		void CreateGraph();
		void LoadGraphInternal();
		void CloseInputFile();
		void InsertNewEdge(int &, char, Edge*);
	private:
		std::ifstream * m_InputFile;
		Graph * m_Graph;
		int m_NumberOfGraphNodes;
};

#endif // __GRAPH_LOADER_H__
