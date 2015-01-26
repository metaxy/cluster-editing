#include "state_branch.h"
#include "randomize.h"
#include <cassert>
using namespace std;


StateBranch::StateBranch(Graph g) : State(g), m_depth(0), m_branchCounter(0)
{
}
MGraph StateBranch::solve(MGraph graph)
{
    m_mgraph = graph;
    while(branch()) {
        m_k++;
    }
    return m_mgraph;
}
int StateBranch::branch()
{
    const P3 p3 = m_mgraph.findNextBestP3(m_depth*0.5);
    const NodeT a = get<0>(p3);
    const NodeT b = get<1>(p3);
    const NodeT c = get<2>(p3);
    if(a == 0 && b == 0 && c==0) {
        //clog << "no p3 found" << endl;
        return 0; //no p3 found
    }
    m_branchCounter++;
    //m_mgraph.writeGraph(to_string(m_branchCounter), p3);
    //clog << "remove p3 = " << a << " " << b << " " << c << endl;

    const Edge e(a,b);
    m_depth += abs(m_mgraph.getWeight(e));
    m_mgraph.flip(e);//removed edge e
    if(m_depth <= m_k) {
        if (branch() == 0) {
            return 0;
        }
    }
    m_mgraph.flip(e);//add edge e
    m_depth -= abs(m_mgraph.getWeight(e));

    m_depth += m_mgraph.merge(e);
    if(m_depth <= m_k) {
        if (branch() == 0) {
            return 0;
        }
    }
    m_depth -= m_mgraph.dismerge(e);
    return 1;
}

