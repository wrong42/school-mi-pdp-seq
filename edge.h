#ifndef __EDGE_H__
#define __EDGE_H__

struct Edge
{
	int Node1;
	int Node2;

	Edge(int node1, int node2) : Node1(node1), Node2(node2) {}
};

#endif __EDGE_H__
