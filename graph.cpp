#include <sstream>
#include <fstream>
#include "graph.h"

Graph::Graph() : nNodes(0), firstEdge(NULL), firstNode(NULL) {}


void Graph::reset()
{
	while (firstEdge != NULL)
	{
		GEdge *e = firstEdge->next;
		delete firstEdge;
		firstEdge = e;
	}
	while (firstNode != NULL)
	{
		GNode *n = firstNode->next;
		delete firstNode;
		firstNode = n;
	}
	nNodes = 0;
}


int Graph::read(char* filename)
{
	if (filename) {ifstream is; is.open(filename); return read((istream&)is);}
	else return read(cin);
}

int Graph::read(istream &infile)
{
    string line, node1, node2;
//    reset();
	while (std::getline(infile, line))
    {
        int i;
        if ((i = line.find_first_of("#")) != string::npos) {
            line = line.substr(0, i);
        }
        stringstream ss(line);
        ss >> node1 >> node2;
        if (node1 == "") continue;
        if (node2 == "") return 0;
        insertEdge(node1, node2);
    }
    return 1;
}


void Graph::intNodes(int n)
{
	for (int i=0; i<n; i++)
	{
		ostringstream node;
		node << i;
		getNodeByName(node.str());
	}
}

int Graph::insertEdge(NodeT u, NodeT v)
{
	ostringstream node1, node2;
	node1 << u;
	node2 << v;
	return insertEdge(node1.str(), node2.str());
}

int Graph::insertEdge(string Node1, string Node2)
{
    int u = getNodeByName(Node1), v = getNodeByName(Node2);
    if (u == v) return 0;
    GEdge *edge = new GEdge(u, v, firstEdge);
    firstEdge = edge;
    return 1;
}

int Graph::getNodeByName(string name)
{
    GNode *node = firstNode;
    for (int i = nNodes; --i >= 0; node = node->next)
        if (name == node->name) return i;
    node = new GNode(name, firstNode);
    firstNode = node;
    nNodes ++;
    return nNodes - 1;
}

string Graph::getNodeByInt(int value)
{
    GNode *node = firstNode;
    if (value >= nNodes) return "unbekannt";
    while (++value < nNodes) node = node->next;
    return node->name;
}

int Graph::nEdges()
{
	int n=0;
	for (GEdge *e=firstEdge; e!=NULL; e=e->next) n++;
	return n;
}

void Graph::output()
{
	for (GEdge *e=firstEdge; e!=NULL; e=e->next)
		cout << getNodeByInt(e->u) << " " << getNodeByInt(e->v) << endl;
}
