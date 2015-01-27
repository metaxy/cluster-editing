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


    virtual MGraph solve(MGraph graph) = 0;
    MGraph solveSurely(MGraph graph);

    virtual void solveFull(int count = 1);

    list<P3> findAllP3s() const;

    void printEdges(vector<Edge> edges);
    void printEdge(const Edge &e);

    void reduceZero(MGraph *graph);
    void reduce2K(MGraph *graph);

    void setConfig(map<string,string> config);
    virtual void parseConfig();


protected:
    double getDouble(const string &name, double def);
    int getInt(const string &name, int def);

    //MGraph m_graphCopy;
    Graph m_input;
    int m_k;

    bool m_flag_reduceZero = false;
    bool m_flag_reduce2K = false;

    map<string,string> m_config;
    int m_seed;

private:
    MGraph m_graph;
};

#endif // STATE2619_H
