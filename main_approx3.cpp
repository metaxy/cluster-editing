#include "state_approx3.h"

int main()
{
    Graph g = Common::graphFromStdin();
    //Graph g = Common::graphFromFile("/home/paul/Cloud/studium/Cluster/programs/material/ce-data/test014.graph");
 //   Graph g = Common::graphFromFile("../../programs/material/ce-data/test014.graph");
    StateApprox3 s0(g);
    s0.solve();
    cout << "#k: " << s0.m_k << endl;
    cout << "#recursive steps: " << s0.m_recsteps << endl;
	return 0;
}
