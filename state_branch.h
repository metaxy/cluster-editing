#ifndef STATE_BRANCH_H
#define STATE_BRANCh_H
#include "graph.h"
#include "state.h"
#include <list>
#include "common.h"
class StateBranch : public State
{
public:
    StateBranch(Graph g);
    MGraph solve(MGraph graph);

    int branch();
    MGraph m_mgraph;

    int m_depth;
    int m_branchCounter;
    P3 findNextBestP3();

};

#endif // STATE2619_H
