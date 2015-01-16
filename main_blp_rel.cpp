#include "state_blp_rel.h"

int main()
{
	Graph g;
	Graph result;
	g.read();

    StateBlpRel s0(&g);
	s0.solveZK(&result);
    cout << "#k: " << s0.m_k << endl;
    cout << "#recursive steps: " << s0.m_recsteps << endl;
	result.output();		
	return 0;
}
