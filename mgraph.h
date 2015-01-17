#ifndef MGRAPH_H
#define MGRAPH_H
#include "common.h"
#include <vector>
class MGraph
{
public:
    MGraph(int nodeCount);
    MGraph(MGraph *copy);
    ~MGraph();
    void addEdge(const Edge &e);
    bool connected(const Edge &e) const;
    int absolut(const Edge &e) const;
    bool isDeleted(NodeT x) const;

    void setWeight(Edge e, int weight);
    int getWeight(Edge e) const;

    int merge(Edge e);
    int dismerge(Edge e);

    vector<NodeT> nodes() const;
    vector<Edge> edges() const;
    Model createModel() const;

    vector<P3> findAllP3s() const;

    static Edge normalize(Edge e);
    static Edge edge(NodeT x, NodeT y);

    int nodeCount() const;
private:
    int m_nodeCount;
    bool connected(NodeT x, NodeT y) const;
    int absolut(NodeT u, NodeT v) const;
    int getWeight(NodeT x, NodeT y) const;
    NodeT **m_matrix;
    NodeT *m_deleted;

};

#endif // MGRAPH_H
