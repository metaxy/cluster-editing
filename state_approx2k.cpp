#include "state_approx2k.h"
using namespace std;


StateApprox2K::StateApprox2K(Graph g) : State(g)
{
}
MGraph StateApprox2K::solve(MGraph graph)
{
    float clique = 1.0;
    float cutting = 1.0;
    float size = 1;
    while(graph.findP3() != P3(0,0,0)) {
        bool someone_merged = false;
        for(NodeT u : graph.nodes()) {
            if(graph.fast2K(u, clique, cutting, size)) {
                set<NodeT> n = graph.neighborhood(u);
                for(NodeT a : n) {
                    for(NodeT b : n) {
                        if(a == b || graph.isDeleted(a) || graph.isDeleted(b)) continue;
                        graph.merge(Edge(a,b));
                        someone_merged = true;
                    }
                }
            }
        }
        /*if(someone_merged == false) {
            return solveSurely(graph);
        }*/
        clique = m_reduce_factor_clique*clique /*- pow(m_reduce_exponent_clique,1/clique)*/ - m_reduce_summand_clique;
        cutting = m_reduce_factor_cutting*cutting /*- pow(m_reduce_exponent_cutting,1/cutting)*/ - m_reduce_summand_cutting;
        size = m_reduce_factor_size*size - m_reduce_summand_size;
        if(clique < 0) clique = 0;
        if(cutting < 0) cutting = 0;
        if(size < 0) size = 0;
    }
    return graph;
}
void StateApprox2K::parseConfig()
{
    m_reduce_factor_clique = getDouble("reduce_factor_clique", 0.95);
    m_reduce_exponent_clique = getDouble("reduce_exponent_clique", 0.01);//between 0 and 0.9
    m_reduce_summand_clique = getDouble("reduce_summand_clique", 0.01);//between 0 and 0.9

    m_reduce_factor_cutting = getDouble("reduce_factor_cutting", 0.95);
    m_reduce_exponent_cutting = getDouble("reduce_exponent_cutting", 0.01);//between 0 and 0.9
    m_reduce_summand_cutting = getDouble("reduce_summand_cutting", 0.01);//between 0 and 0.9

    m_reduce_factor_size = getDouble("reduce_factor_size", 1);
    m_reduce_summand_size = getDouble("reduce_summand_size", 0);
}
