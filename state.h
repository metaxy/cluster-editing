#ifndef STATE_H
#define STATE_H
#include "graph.h"
#include "mgraph.h"
#include "common.h"
#include <list>
#include <vector>
class State
{
public:

    State(Graph g);
    ~State();

    void branch();
    void solveZK(Graph *result);

    virtual MGraph solve(MGraph graph) = 0;

    virtual void solveFull();

    unsigned int m_k;
    unsigned int m_recsteps;

    list<P3> findAllP3s() const;

    void printEdges(vector<Edge> edges);
    void printEdge(const Edge &e);

    void reduceZero(MGraph *graph);

    void setConfig(map<string,string> config);

protected:
    //MGraph m_graphCopy;
    Graph m_input;
    int m_depth;


    bool m_flag_reduceZero = false;
private:
    MGraph m_graph;
};

#endif // STATE2619_H
