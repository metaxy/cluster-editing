#ifndef STATE_BLP_H
#define STATE_BLP_H
#include "graph.h"
#include "state.h"
#include <list>
#include "common.h"
class StateBlp : public State
{
public:
    StateBlp(Graph g);
    MGraph solve(MGraph graph);
};

#endif // STATE2619_H
