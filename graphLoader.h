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
		int m_NumberOfGraphNodes;
		Graph * m_Graph;
		
		std::ifstream * m_InputFile;
		std::vector<Edge*> m_Edges;
};

#endif // __GRAPH_LOADER_H__
