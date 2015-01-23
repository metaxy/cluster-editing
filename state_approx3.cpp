#include "state_approx3.h"
#include "randomize.h"
using namespace std;


StateApprox3::StateApprox3(Graph g) : State(g)
{
}
MGraph StateApprox3::solve(MGraph graph)
{
    Randomize r;

    MGraph res(&graph);
    res.clear();

    set<NodeT> nodes = graph.nodesSet();
    while(!nodes.empty()) {
        NodeT u = r.randomElement(nodes);
        set<NodeT> cluster = graph.neighborhood(u);
        cluster.insert(u);
        cluster = set_intersect(cluster, nodes);
        for(NodeT v : cluster) {
            for(NodeT w : cluster) {
                if(v == w) continue;
                res.setWeight(Edge(v,w), 1);
            }
            nodes.erase(v);
        }
    }
    return res;
}
