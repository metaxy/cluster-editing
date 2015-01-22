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

    MGraph res(m_input);

    set<NodeT> nodes = m_graph->nodesSet();
    while(!nodes.empty()) {
        NodeT u = r.randomElement(nodes);
        set<NodeT> cluster = m_graph->neighborhood(u);
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
    printEdges(m_graph->difference(&res));
}
