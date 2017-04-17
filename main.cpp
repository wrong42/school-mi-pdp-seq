#include <iostream>
#include <cstring>
#include <queue>
#include <set>
#include <omp.h>
#include <mpi.h>
#include "graphLoader.h"
#include "bigraphMaker.h"
#include "graph.h"
#include "edge.h"

using namespace std;

#define NUMBER_OF_THREADS 4

enum MPI_TAGS {WORK = 0, DONE = 1, FINISHED = 2, UPDATE_BG = 3, INIT_GRAPH = 4};

Graph * m_LoadedGraph;
BigraphMaker m_BigraphMaker;

MPI_Status status;
MPI_Request sendRequest, recieveRequest;

int bestGraphUpdate, flag;
int m_MaxFoundBigraphByEdge = 0;
int proc_num, numberOfProceses;

void DeleteGraphs(Graph * loadedGraph)
{	
	for (unsigned i = 0; i < loadedGraph->m_NumberOfEdgesOriginal; i++)
	{
		delete loadedGraph->m_Edges[i];
	}
	
	delete[] loadedGraph->m_Edges;
	delete loadedGraph;
}

void FindMaxBigraphInternal(Graph graph)
{
	if (graph.m_NumberOfEdgesCurrent < graph.m_NumberOfNodes - 1)
	{
		return;
	}

	// Test if one of the other processes found a better solution
	MPI_Test(&recieveRequest, &flag, &status);
	if (flag)
	{
		cout << proc_num << ". Recieved UPDATE_BG from MASTER - edges: " << bestGraphUpdate << endl;
		#pragma omp atomic write
			m_MaxFoundBigraphByEdge = bestGraphUpdate;

		MPI_Irecv(&bestGraphUpdate, 1, MPI_INT, 0, UPDATE_BG, MPI_COMM_WORLD, &recieveRequest);
	}

	/********** COLORING GRAPH SECTION **********/
	queue<int> m_ColoredNodes;
	set<int> m_ProcessedNodes;

	graph.m_NodeColors[0] = White;
	m_ColoredNodes.push(0);
	bool coloringSuccessful = true;

	while(!m_ColoredNodes.empty())
	{
		int nodeIndex = m_ColoredNodes.front();
		m_ColoredNodes.pop();

		Color neighbourColor = graph.m_NodeColors[nodeIndex] == Black ? White : Black;
		
		if (!graph.ColorNeighbourNodes(nodeIndex, neighbourColor))
		{
			//cout << "BigraphMaker::ColorNodes: UNABLE TO COLOR GRAPH" << endl;
			coloringSuccessful = false;
			break;
		}

		m_ProcessedNodes.insert(nodeIndex);

		for (int i = 0; i < graph.m_NumberOfNodes; i++)
		{
			if (graph.AreNeighbours(nodeIndex, i))
			{
				if (m_ProcessedNodes.find(i) == m_ProcessedNodes.end())
				{
					//cout << "BigraphMaker::ColorNodes: Pushing not processed node: " << i << " to nodeToColor queue" << endl;
					m_ColoredNodes.push(i);
				}
			}
		}

		if (m_ColoredNodes.empty())
		{
			int notColoredNode = graph.GetFirstUncoloredNode(); 
			//cout << "BigraphMaker::ColorNodes: Nodes to color queue is empty. Index of first not yet colored node: " << notColoredNode << endl;
			if (notColoredNode > -1)
			{
				graph.m_NodeColors[notColoredNode] = White;
				m_ColoredNodes.push(notColoredNode);
			}
		}
	}

	/**********END OF COLORING GRAPH SECTION **********/

	if (coloringSuccessful)
	{
		#pragma omp atomic write
			m_MaxFoundBigraphByEdge = graph.m_NumberOfEdgesCurrent;

		//cout << proc_num << ". Found BETTER graph - edeges: " << graph.m_NumberOfEdgesCurrent << endl; 
		MPI_Isend(&m_MaxFoundBigraphByEdge, 1, MPI_INT, 0, UPDATE_BG, MPI_COMM_WORLD, &sendRequest);
		return;
	}
	else if (m_ColoredNodes.size() == 0 && m_ProcessedNodes.size() < graph.m_NumberOfNodes)
	{
		return;
	}

	int numberOfBestGraphEdges = 0;
	
	#pragma omp atomic read
	numberOfBestGraphEdges = m_MaxFoundBigraphByEdge;

	if (graph.m_NumberOfEdgesCurrent - 1 > numberOfBestGraphEdges)
	{
		for (unsigned i = graph.m_LastErasedEdge + 1; i < graph.m_NumberOfEdgesOriginal; i++)
		{
			Graph childGraph(graph);
			childGraph.RemoveEdge(i);
			
			#pragma omp task
				FindMaxBigraphInternal(childGraph);
		}
	}
}

int main(int argc, char * argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_num);
	MPI_Comm_size(MPI_COMM_WORLD, &numberOfProceses);

	//cout << proc_num << ". Start of application. Number of arguments: " << argc << endl;
	if (argc < 2)
	{
		cout << proc_num << ". No input file selected. Closing the application." << endl;
		return 1;
	}

		/* LOAD GRAPH FROM FILE */
		//cout <<  proc_num << ". Loading graph." << endl;
		GraphLoader loader;
		m_LoadedGraph = loader.LoadGraph(argv[1]);
		//cout << proc_num << ". Loaded Graph: NumberOfEdges = " << m_LoadedGraph->m_NumberOfEdgesOriginal << endl;
		//m_LoadedGraph->Print();	


	if (proc_num == 0)
	{
		// Try original graph
		if (m_BigraphMaker.MakeBigraph(*m_LoadedGraph))
		{
			cout << "ORIGINAL GRAPH IS RESULT" << endl;
			return 0;
		}

		int removedEdgeIndex = 0;

		for (int i = 1; i < numberOfProceses; i++, removedEdgeIndex++)
		{
			//cout << proc_num << ". Sending msg to slave " << i << endl;
			MPI_Send(&removedEdgeIndex, 1, MPI_INT, i, WORK, MPI_COMM_WORLD);
		}

		int bestGraphBySlave, slavesWorking = numberOfProceses - 1;

		while (slavesWorking > 0)
		{
			MPI_Recv(&bestGraphBySlave, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			//cout << proc_num << ". SLAVES WORKING: " << slavesWorking << " (before switch)" << endl;
			switch(status.MPI_TAG)
			{
				case UPDATE_BG:
				{
					cout << proc_num << ". Recieved UPDATE_BG from slave " << status.MPI_SOURCE << " - edges: " << bestGraphBySlave << endl;
					if (bestGraphBySlave > m_MaxFoundBigraphByEdge)
					{
						m_MaxFoundBigraphByEdge = bestGraphBySlave;
						// UPDATE BEST GRAPH TO SLAVES
						for (int i = 1; i < numberOfProceses; i++)
						{
							if (i != status.MPI_SOURCE)
								MPI_Isend(&bestGraphBySlave, 1, MPI_INT, i, UPDATE_BG, MPI_COMM_WORLD, &sendRequest);
						}
					}
					break;
				}
				case DONE:
				{
					if (bestGraphBySlave > m_MaxFoundBigraphByEdge)
					{
						m_MaxFoundBigraphByEdge = bestGraphBySlave;
						// UPDATE BEST GRAPH TO SLAVES
						for (int i = 1; i < numberOfProceses; i++)
						{
							MPI_Send(&bestGraphBySlave, 1, MPI_INT, i, UPDATE_BG, MPI_COMM_WORLD);
						}
						// UPDATE MY BEST GRAPH
					}
					if (removedEdgeIndex < m_LoadedGraph->m_NumberOfEdgesOriginal)
					{
						MPI_Send(&removedEdgeIndex, 1, MPI_INT, status.MPI_SOURCE, WORK, MPI_COMM_WORLD);
						removedEdgeIndex++;
						break;			
					}
					else
					{
						MPI_Send(&removedEdgeIndex, 1, MPI_INT, status.MPI_SOURCE, FINISHED, MPI_COMM_WORLD);
						--slavesWorking;
					}

					break;
				}
				default:
					cout << "WTF SHOULD I DO NOW?" << endl;
			}

			//cout << proc_num << ". SLAVES WORKING: " << slavesWorking << " (after switch)" << endl;
		}

		/* GET MAX BIPARTHITE GRAPH */
		cout << proc_num << ". ALL SLAVES HAVE ENDED " << endl;
		cout << "RESULT Graph: NumberOfEdges = " << m_MaxFoundBigraphByEdge << endl;

		/* DELETE SECTION */
		DeleteGraphs(m_LoadedGraph);
		MPI_Finalize();
		return 0;
	}
	else
	{
		int msg, maxNumberOfEdges = 0;
		while(true)
		{
			//cout << proc_num << ". WAITING FOR MSG" << endl;
			MPI_Recv(&msg, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);	
			//cout << proc_num << ". Msg recieved with tag: " << status.MPI_TAG << endl;
			switch(status.MPI_TAG) 
			{
				case WORK:
				{
					//cout << proc_num << ". Recieved message to work: edgeToRemove = " << msg << endl;
					Graph graphToProcess(*m_LoadedGraph);
					graphToProcess.RemoveEdge(msg);

					MPI_Irecv(&bestGraphUpdate, 1, MPI_INT, 0, UPDATE_BG, MPI_COMM_WORLD, &recieveRequest);
					#pragma omp parallel num_threads(NUMBER_OF_THREADS)
					{
						#pragma omp single
							FindMaxBigraphInternal(graphToProcess);
					}

					//cout << proc_num << ". RESULT - best graph - number of edges = " << m_MaxFoundBigraphByEdge << endl;
					MPI_Send(&m_MaxFoundBigraphByEdge, 1, MPI_INT, 0, DONE, MPI_COMM_WORLD);
					break;
				}
				case UPDATE_BG:
					cout << proc_num << ". Recieved UPDATE_BG from MASTER: edges" << msg << endl;
					m_MaxFoundBigraphByEdge = msg;
					break;
				case FINISHED:
					cout << proc_num << ". RECIEVED FINISHED TAG" << endl;
					MPI_Finalize();
					return 0;
				default:
					cout << proc_num << ". DONT KNOW THIS TAG: " << status.MPI_TAG << endl;
					return -1;
					break;
			}
		}
	}
}