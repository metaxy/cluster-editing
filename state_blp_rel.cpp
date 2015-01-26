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
                    if(r.choice(0.5))
                        graph.setWeight(i.first, 1-i.second);
                }
                counter++;
                if(counter > 100) return graph;
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

