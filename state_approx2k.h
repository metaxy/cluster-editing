#ifndef STATE_APPROX_2K_H
#define STATE_APPROX_2K_H
#include "graph.h"
#include "state.h"
#include <list>
#include "common.h"
class StateApprox2K : public State
{
public:
     StateApprox2K(Graph g);
     MGraph solve(MGraph graph);

};

#endif // STATE_APPROX_2K_H
