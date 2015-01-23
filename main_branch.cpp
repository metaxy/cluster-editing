#include "state_branch.h"

int main(int argc, char* argv[])
{
    Graph g = Common::graphFromStdin();
    //Graph g = Common::graphFromFile("/home/paul/Cloud/studium/Cluster/programs/material/ce-data/test004.graph");
   // Graph g = Common::graphFromFile("../../programs/material/ce-data/test014.graph");
    StateBranch s0(g);
    s0.setConfig(Common::parseConfig(argc, argv));
    s0.solveFull();
	return 0;
}
