#include "state_lp_round.h"
#include <cassert>
#include "gurobilp.h"
#include "randomize.h"
using namespace std;

#define F_A 0.19
#define F_B 0.5095

StateLpRound::StateLpRound(Graph *g) : State(g)
{
}
int StateLpRound::solve(Graph *result)
{
    try {
        GurobiLP g(m_nodeCount);
        Randomize r;
        Model model = createModel();
        g.addModelVarsRelaxed(model);
        g.setObjective(model);

        list<P3> knownP3 = findAllP3s();
        list<Edge> res;
        while(!knownP3.empty()) {
            res.clear();
            g.addConstraints(knownP3);
            
            ModelRelaxed ret = g.optimizeRelaxed();
            
            vector<NodeT> V = nodes();
            while(!V.empty()) {
                NodeT u = r.randomElement(V);
                std::list<NodeT> C;
                for(const NodeT v : V) {
                    Edge e = Common::edge(u,v);
                    float prob = 1 - ret[e];
                    //clog << prob; 
                    if(getWeight(e) > 0) {
                        prob = fplus(prob);
                    }
                    //clog << u << " " << v << " prob = " << 1-prob << endl;
                    if(r.choice(1-prob)) {
                        C.push_back(v);
                        res.push_back(Edge(u,v));
                    }
                }

                for(const NodeT v: C) {
                    m_deleted[v] = 1;
                }
                V = nodes();
            }
            knownP3 = findAllP3s();
        }
        for(Edge e: res) {
                cout << m_graph->getNodeByInt(e.first) << " " << m_graph->getNodeByInt(e.second) << endl;
        }

       // output(result);
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
