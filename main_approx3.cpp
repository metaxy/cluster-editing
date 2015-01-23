#include "state_approx3.h"

int main()
{
  //  Graph g = Common::graphFromStdin();
    Graph g = Common::graphFromFile("/home/paul/Cloud/studium/Cluster/programs/material/ce-data/test001.graph");
 //   Graph g = Common::graphFromFile("../../programs/material/ce-data/test014.graph");
    StateApprox3 s0(g);
    s0.solveFull();
	return 0;
}
