#ifndef GRAPH_H
#define GRAPH_H
#include <string>
#include <cstring>
#include <iostream>
#include <map>
using namespace std;
typedef int NodeT;
typedef pair<NodeT,NodeT> Edge;
typedef tuple<NodeT,NodeT,NodeT> P3;
typedef map<Edge,int> Model;
typedef map<Edge,float> ModelRelaxed;

class GNode
{
public:
    string name;
    GNode *next;
    GNode(string n, GNode *node) {name = n; next = node;}
};

class GEdge
{
public:
    NodeT u;
    NodeT v;
    GEdge *next;
    GEdge(NodeT i, NodeT j, GEdge *e) {u = i; v = j; next = e;}
    Edge toEdge() {
        return Edge(u,v);
    }
};

class Graph
{
public:
    int nNodes;
    int k;
    GEdge *firstEdge;
    GNode *firstNode;

    Graph();
    void reset();
    int read(char *filename = NULL);
	int read(istream &is);
	void intNodes(int n);
	void build(int nn, int nc, int m1, int m2, Graph *delta=NULL);
    int insertEdge(NodeT Node1, NodeT Node2);
    int insertEdge(string Node1, string Node2);
    int getNodeByName(string Name);
    string getNodeByInt(int value);
    int nEdges();
    void output();
};

#endif // GRAPH_H
