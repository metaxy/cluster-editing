#include "state_blp.h"
#include <cassert>
#include "gurobilp.h"
using namespace std;

StateBlp::StateBlp(Graph *g) : State(g)
{
}

int StateBlp::solve(Graph *result)
{
    try {
        GurobiLP g(m_nodeCount);
        Model model = createModel();
        g.addModelVars(model);
        g.setObjective(model);

        list<P3> knownP3 = findAllP3s();
        while(!knownP3.empty()) {
            g.addConstraints(knownP3);
            m_recsteps += knownP3.size()*3;
            Model ret = g.optimize();
            for(const auto &i: ret) {
                Edge e = i.first;
                m_matrix[e.first][e.second] = i.second;
                m_matrix[e.second][e.first] = i.second;
            }
            knownP3 = findAllP3s();
        }
        output(result);
    } catch(GRBException e) {
        clog << "Error code = " << e.getErrorCode() << endl;
        clog << e.getMessage() << endl;
        exit(-1);
    } catch(...) {
        clog << "Exception during optimization" << endl;
        exit(-1);
    }
}

