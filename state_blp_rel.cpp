#include "state_blp_rel.h"
#include <cassert>
#include "gurobilp.h"
using namespace std;
StateBlpRel::StateBlpRel(Graph *g) : State(g)
{
}
int StateBlpRel::solve(Graph *result)
{
    try {
        GurobiLP g(m_nodeCount);
        Model model = createModel();
        g.addModelVarsRelaxed(model);
        g.setObjective(model);

        list<P3> knownP3 = findAllP3s();
        while(!knownP3.empty()) {
            g.addConstraints(knownP3);
            m_recsteps += knownP3.size()*3;
            clog << "new step " << knownP3.size() << endl;
            Model ret = g.optimize();
            for(const auto &i: ret) {
                setWeight(i.first, i.second);
            }
            knownP3 = findAllP3s();
        }
        output(result);
    } catch(GRBException e) {
        clog << "Error code = " << e.getErrorCode() << endl;
        clog << e.getMessage() << endl;
    } catch(...) {
        clog << "Exception during optimization" << endl;
    }
}

