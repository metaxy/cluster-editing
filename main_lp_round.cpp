#include "state_lp_round.h"

int main()
{
	Graph g;
	g.read();

    StateLpRound s0(g);
    s0.solve();
    cout << "#k: " << s0.m_k << endl;
    cout << "#recursive steps: " << s0.m_recsteps << endl;	
	return 0;
}
