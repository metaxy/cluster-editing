#include "state_blp.h"
#include <cassert>
#include "gurobilp.h"
using namespace std;

StateBlp::StateBlp(Graph g) : State(g)
{
}

MGraph StateBlp::solve(MGraph graph)
{
    try {
        GurobiLP g(graph.nodeCount());
        Model model = graph.createModel();
        g.addModelVars(model);
        g.setObjective(model);

        vector<P3> knownP3 = graph.findAllP3s();
        while(!knownP3.empty()) {
            g.addConstraints(knownP3);
            m_recsteps += knownP3.size()*3;
            Model ret = g.optimize();
            for(const auto &i: ret) {
                graph.setWeight(i.first, i.second);
            }
            knownP3 = graph.findAllP3s();
        }
        return graph;
    } catch(GRBException e) {
        clog << "Error code = " << e.getErrorCode() << endl;
        clog << e.getMessage() << endl;
        exit(-1);
    } catch(...) {
        clog << "Exception during optimization" << endl;
        exit(-1);
    }
}

