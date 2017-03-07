#ifndef __EDGE_H__
#define __EDGE_H__

struct Edge
{
	int ID;
	int Node1;
	int Node2;

	Edge(int id, int node1, int node2) : ID(id), Node1(node1), Node2(node2) {}
};

#endif // __EDGE_H__
