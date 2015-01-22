#ifndef STATE_APPROX_3_H
#define STATE_APPROX_3_H
#include "graph.h"
#include "state.h"
#include <list>
#include "common.h"
class StateApprox3 : public State
{
public:
     StateApprox3(Graph g);
     int solve();

};

#endif // STATE2619_H
