#include "state_blp_rel.h"

int main()
{
	Graph g;
	g.read();

    StateBlpRel s0(g);
    s0.solve();
    cout << "#k: " << s0.m_k << endl;
    cout << "#recursive steps: " << s0.m_recsteps << endl;		
	return 0;
}
