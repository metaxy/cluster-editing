#ifndef STATE_H
#define STATE_H
#include "graph.h"
#include "common.h"
#include <list>
#include <vector>
class State
{
public:

    State(Graph *g, int db=0);
    ~State();
    int absolut(NodeT u, NodeT v) const;
    bool isDeleted(NodeT x) const;
    bool connected(NodeT x, NodeT y) const;
    void setWeight(Edge e, int weight);
    bool hasSameNeighbours(NodeT u, NodeT v);
	int mergeCost(NodeT u, NodeT v);
    void merge(NodeT u, NodeT v);
    void dismerge();
    bool mergeTwin();
    int mergeTwins();
    int core(NodeT u);
    bool core();
    void normalize();
    void prepare();
    int findBestP3(NodeT &a, NodeT &b, int needed);
    void branch(int start, int k);
    void solveZK(Graph *result);
    void output(Graph *result);
    void printState();
    bool isHeavy(NodeT, NodeT);
    bool heavyEdge();
    bool heavyEdges();

    State * clone(Graph *g);
    virtual int solve(Graph *result) = 0;

    unsigned int m_k;
    unsigned int m_recsteps;

    vector<NodeT> nodes() const;
    list<P3> findAllP3s() const;

    Model createModel() const;
    int getWeight(Edge e) const;
protected:
    int debug;
    Graph *m_graph;
    NodeT **m_matrix;
    NodeT *m_matrix_copy;
    unsigned int m_nodeCount;
    NodeT *m_deleted;
    int *m_nodeWeight;
    NodeT *m_merged;
    NodeT *m_CC;
    NodeT *m_bestresult;
    int m_tn;
    int m_tnc;
    int m_nCore;
    int m_nDivisions;

};

#endif // STATE2619_H
