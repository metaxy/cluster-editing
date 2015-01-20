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
int StateLpRound::solve()
{
    try {
        GurobiLP g(m_graph->nodeCount());
        Randomize r;
        Model model = m_graph->createModel();
        g.addModelVarsRelaxed(model);
        g.setObjective(model);

        MGraph res(m_graph->nodeCount());
        for(int u = 0; u <m_graph->nodeCount(); u++) {
            for(int v = u+1; v <m_graph->nodeCount(); v++) {
                for(int w = v+1; w <m_graph->nodeCount(); w++) {
                    if(!m_graph->isDeleted(u) && !m_graph->isDeleted(v) && !m_graph->isDeleted(w)) {
                        g.addConstraint(P3(u,v,w));
                    }
                }
            }
        }
        //g.addConstraints(knownP3);
        
        ModelRelaxed ret = g.optimizeRelaxed();
        set<NodeT> V = m_graph->nodesSet();
        while(!V.empty()) {
            //clog << "V.size = " <<  V.size() << endl;
            NodeT u = r.randomElement(V);
            //clog << "random element " << u << endl; 
            std::list<NodeT> C;
            for(const NodeT v : V) {
                Edge e = MGraph::edge(u,v);
                double prob = 1 - ret[e];
        //        clog << ret[e] << endl;
                if(m_graph->getWeight(e) > 0) {
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
        printEdges(m_graph->difference(&res));
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
