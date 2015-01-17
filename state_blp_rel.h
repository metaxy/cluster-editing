#ifndef STATE_BLP_REL_H
#define STATE_BLP_REL_H
#include "graph.h"
#include "state.h"
#include <list>
#include "common.h"
class StateBlpRel : public State
{
public:
     StateBlpRel(Graph g);
     int solve();

};

#endif // STATE2619_H
