#include "state_lp_round.h"
#include <cassert>
#include "gurobilp.h"
#include "randomize.h"
#include <algorithm>
using namespace std;

#define F_A 0.19
#define F_B 0.5095

StateLpRound::StateLpRound(Graph g) : State(g)
{
}
MGraph StateLpRound::solve(MGraph graph)
{
    try {
        GurobiLP g(graph.nodeCount());
        Randomize r;
        Model model = graph.createModel();
        g.addModelVarsRelaxed(model);
        g.setObjective(model);

        MGraph res(graph.nodeCount());
        for(int u = 0; u <graph.nodeCount(); u++) {
            for(int v = u+1; v <graph.nodeCount(); v++) {
                for(int w = v+1; w <graph.nodeCount(); w++) {
                    if(!graph.isDeleted(u) && !graph.isDeleted(v) && !graph.isDeleted(w)) {
                        g.addConstraint(P3(u,v,w));
                    }
                }
            }
        }
        //g.addConstraints(knownP3);
        
        ModelRelaxed ret = g.optimizeRelaxed();
        set<NodeT> V = graph.nodesSet();
        while(!V.empty()) {
            //clog << "V.size = " <<  V.size() << endl;
            NodeT u = r.randomElement(V);
            //clog << "random element " << u << endl; 
            std::list<NodeT> C;
            for(const NodeT v : V) {
                Edge e = MGraph::edge(u,v);
                double prob = 1 - ret[e];
        //        clog << ret[e] << endl;
                if(graph.getWeight(e) > 0) {
                    prob = fplus(prob);
                }
                if(r.choice(1-prob)) {
                    C.push_back(v);
                }
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
        return graph;
    } catch(GRBException e) {
        clog << "Error code = " << e.getErrorCode() << endl;
        clog << e.getMessage() << endl;
    } catch(...) {
        clog << "Exception during optimization" << endl;
    }
}

float StateLpRound::fplus(float x) const
{
    if(x < F_A) return 0;
    if(x >= F_B) return 1;
    return ((x-F_A)*(F_B-F_A))*((x-F_A)*(F_B-F_A));

}
