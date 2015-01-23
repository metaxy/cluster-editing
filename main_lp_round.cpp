#include "state_lp_round.h"

int main()
{
	Graph g;
	g.read();

    StateLpRound s0(g);
    s0.solveFull();
	return 0;
}
