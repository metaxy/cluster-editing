#ifndef GUROBILP_H
#define GUROBILP_H
#include "gurobi_c++.h"
#include "graph.h"
#include "common.h"
#include <map>
#include <list>

using namespace std;

class GurobiLP
{
public:
    GurobiLP(int nodeCount);
    void setObjective(Model weights);
    void addVars(list<Edge>);
    void addModelVars(Model weights);
    void addModelVarsRelaxed(Model weights);
    void addConstraint(P3 p3);
    void addConstraints(list<P3> p3s);
    Model optimize();
    ModelRelaxed optimizeRelaxed();
private:
    GRBEnv m_env = GRBEnv();
    GRBModel m_model = GRBModel(m_env);
    int m_nodeCount;

    GRBVar **m_vars;
    inline GRBVar e(NodeT x, NodeT y);
    inline GRBVar e(Edge);
    inline int weight(NodeT x, NodeT y);
    inline float weightRelaxed(NodeT x, NodeT y);
    inline int weight(Edge);
    inline float weightRelaxed(Edge);
    void addVar(Edge e);
    void addVarRelaxed(Edge e);

    map<Edge,int> m_weights;

};

#endif // GUROBILP_H
