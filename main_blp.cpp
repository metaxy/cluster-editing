#include "state_blp.h"

int main()
{
    Graph g = Common::graphFromStdin();
    //Graph g = Common::graphFromFile("/home/paul/Cloud/studium/Cluster/programs/material/ce-data/test014.graph");
 //   Graph g = Common::graphFromFile("../../programs/material/ce-data/test014.graph");
    StateBlp s0(g);
    s0.solve();
	return 0;
}
