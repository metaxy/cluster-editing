#include "state_blp_rel.h"

int main()
{
	Graph g;
	g.read();

    StateBlpRel s0(g);
    s0.solve();
	return 0;
}
