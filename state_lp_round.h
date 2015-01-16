#ifndef STATE_LP_ROUND_H
#define STATE_LP_ROUND_H
#include "graph.h"
#include "state.h"
#include <list>
#include "common.h"
class StateLpRound : public State
{
public:
     StateLpRound(Graph *g);
     int solve(Graph *result);
     float fplus(float x) const;

};

#endif // STATE2619_H
