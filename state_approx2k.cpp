#include "state_approx2k.h"
#include "randomize.h"
using namespace std;


StateApprox2K::StateApprox2K(Graph g) : State(g)
{
}
MGraph StateApprox2K::solve(MGraph graph)
{
    Randomize r;
    float a = 1.0;
    while(graph.findP3() != P3(0,0,0)) {
        for(NodeT u : graph.nodes()) {
            set<NodeT> n = graph.neighborhood(u);
            if(2*a*graph.costMakingClique(u) + a*graph.costCutting(u) < n.size()) {
                for(NodeT a : n) {
                    for(NodeT b : n) {
                        if(a == b || graph.isDeleted(a) || graph.isDeleted(b)) continue;
                        graph.merge(Edge(a,b));
                    }
                }
            }
        }
        a = 0.99*a - 0.1;
    }
    return graph;
}
