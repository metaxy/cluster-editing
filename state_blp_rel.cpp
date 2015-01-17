#include "state_blp_rel.h"
#include <cassert>
#include "gurobilp.h"
using namespace std;
StateBlpRel::StateBlpRel(Graph g) : State(g)
{
}
int StateBlpRel::solve()
{
    try {
        GurobiLP g(m_graph->nodeCount());
        Model model = m_graph->createModel();
        g.addModelVarsRelaxed(model);
        g.setObjective(model);

        vector<P3> knownP3 = m_graph->findAllP3s();
        while(!knownP3.empty()) {
            g.addConstraints(knownP3);
            m_recsteps += knownP3.size()*3;
            clog << "new step " << knownP3.size() << endl;
            Model ret = g.optimize();
            for(const auto &i: ret) {
                m_graph->setWeight(i.first, i.second);
            }
            knownP3 = m_graph->findAllP3s();
        }
    } catch(GRBException e) {
        clog << "Error code = " << e.getErrorCode() << endl;
        clog << e.getMessage() << endl;
    } catch(...) {
        clog << "Exception during optimization" << endl;
    }
}

