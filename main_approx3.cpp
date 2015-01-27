#include "state_approx3.h"

int main(int argc, char* argv[])
{
    Graph g = Common::graphFromStdin();
   // Graph g = Common::graphFromFile("/home/paul/Cloud/studium/Cluster/programs/material/ce-data/test001.graph");
 //   Graph g = Common::graphFromFile("../../programs/material/ce-data/test014.graph");
    StateApprox3 s0(g);
    s0.setConfig(Common::parseConfig(argc, argv));
    s0.solveFull(100);
	return 0;
}
