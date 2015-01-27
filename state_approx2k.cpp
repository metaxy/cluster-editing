#include "state_approx2k.h"
#include "randomize.h"
using namespace std;


StateApprox2K::StateApprox2K(Graph g) : State(g)
{
}
MGraph StateApprox2K::solve(MGraph graph)
{
    Randomize r;
    float clique = 1.0;
    float cutting = 1.0;
    while(graph.findP3() != P3(0,0,0)) {
        for(NodeT u : graph.nodes()) {
            set<NodeT> n = graph.neighborhood(u);
            if(2*clique*graph.costMakingClique(u) + cutting*graph.costCutting(u) < m_reduce_factor_size*n.size()) {
                for(NodeT a : n) {
                    for(NodeT b : n) {
                        if(a == b || graph.isDeleted(a) || graph.isDeleted(b)) continue;
                        graph.merge(Edge(a,b));
                    }
                }
            }
        }
        clique = m_reduce_factor_clique*clique - pow(m_reduce_exponent_clique,1/clique);
        cutting = m_reduce_factor_cutting*cutting - pow(m_reduce_exponent_cutting,1/cutting);
    }
    return graph;
}
void StateApprox2K::parseConfig()
{
    m_reduce_factor_clique = getDouble("reduce_factor_clique", 0.9);
    m_reduce_exponent_clique = getDouble("reduce_exponent_clique", 0.01);//between 0 and 0.9

    m_reduce_factor_cutting = getDouble("reduce_factor_cutting", 0.9);
    m_reduce_exponent_cutting = getDouble("reduce_exponent_cutting", 0.01);//between 0 and 0.9

    m_reduce_factor_size = getDouble("reduce_factor_size", 1);
}
