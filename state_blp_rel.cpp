#include "state_blp_rel.h"
#include <cassert>
#include "gurobilp.h"
#include "randomize.h"
using namespace std;
StateBlpRel::StateBlpRel(Graph g) : State(g)
{
}
MGraph StateBlpRel::solve(MGraph graph)
{
    try {
        MGraph res(graph);
        res.clear();
        MGraph copy(graph);
        GurobiLP g(graph.nodeCount());
        Model model = graph.createModel();
        g.addModelVarsRelaxed(model);
        g.setObjective(model);
        Randomize r;
        vector<P3> knownP3 = graph.findAllP3s();
        int lastSize = knownP3.size();
        int counter = 0;
        while(!knownP3.empty()) {
            g.addConstraints(knownP3);
            Model ret = g.optimize();
            for(const auto &i: ret) {
                graph.setWeight(i.first, i.second);
            }
            knownP3 = graph.findAllP3s();
            if(lastSize == knownP3.size()) {
                for(const auto &i: ret) {
                    if(r.choice(0.1))
                        graph.setWeight(i.first, 1-i.second);
                }
                counter++;
                if(counter > 2) {
                    set<NodeT> nodes = copy.nodesSet();
                    while(!nodes.empty()) {
                        NodeT u = r.randomElement(nodes);
                        set<NodeT> cluster = copy.closedNeighborhood(u);
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
            }
            lastSize = knownP3.size();
        }
        return graph;
    } catch(GRBException e) {
        clog << "Error code = " << e.getErrorCode() << endl;
        clog << e.getMessage() << endl;
    } catch(...) {
        clog << "Exception during optimization" << endl;
    }
}

