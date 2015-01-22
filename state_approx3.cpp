#include "state_approx3.h"
#include <cassert>
#include "randomize.h"
#include <algorithm>
using namespace std;


StateApprox3::StateApprox3(Graph g) : State(g)
{
}
int StateApprox3::solve()
{
    Randomize r;

    MGraph res(m_graph->nodeCount());

    set<NodeT> V = m_graph->nodesSet();
    while(!V.empty()) {
        NodeT u = r.randomElement(V);
        std::list<NodeT> C;
        for(const NodeT v : V) {
        }
        V.erase(u);
        //output C
        for(NodeT y : C) {
            V.erase(y);
            for(NodeT v : C) {
                if(y == v) continue;
                res.setWeight(Edge(y,v), 1);
                V.erase(v);
            }
        }
    }
    printEdges(m_graph->difference(&res));
}
