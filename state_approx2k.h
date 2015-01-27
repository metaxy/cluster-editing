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


     void parseConfig();
private:
     double m_reduce_factor_clique;
     double m_reduce_exponent_clique;
     double m_reduce_summand_clique;

     double m_reduce_factor_cutting;
     double m_reduce_exponent_cutting;
     double m_reduce_summand_cutting;

     double m_reduce_factor_size;
     double m_reduce_summand_size;

};

#endif // STATE_APPROX_2K_H
