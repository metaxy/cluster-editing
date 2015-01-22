#include "state.h"
#include <cassert>
#include <sstream>

using namespace std;

State::State(Graph g) : m_input(g), m_graph(new MGraph(g))
{
    m_k = 0;
    m_recsteps = 0;
    
    for (GEdge *e = g.firstEdge; e != NULL; e = e->next) {
       m_graph->addEdge(e->toEdge());
    }
    m_graphCopy = new MGraph(m_graph);

}
State::~State()
{
    delete m_graph;
    delete m_graphCopy;
}
void State::branch()
{
    /*const P3 p3 = this->findNextBestP3();
    const NodeT a = get<0>(p3);
    const NodeT b = get<1>(p3);
    const NodeT c = get<2>(p3);
    if(a == 0 && b == 0 && c==0) {
        return 0; //no p3 found
    }

    const Edge e(a,b);
    m_depth += abs(m_matrix[a][b]);
    flip(e);//removed edge e
    if(this->m_depth <= m_k) {
        if (this->branch(currentBranch) == 0) {
            return 0;
        }
    }
    flip(e);//add edge e
    m_depth -= abs(m_matrix[a][b]);

    merge(e);
    if(this->m_depth <= m_k) {
        if (this->branch(currentBranch) == 0) {
            return 0;
        }
    }
    dismerge(e);
    return 1;
*/
}

void State::printState()
{
}

void State::printEdge(const Edge &e)
{
    cout << m_input.getNodeByInt(e.first) << " " << m_input.getNodeByInt(e.second) << endl;
}

void State::printEdges(vector<Edge> edges)
{
    for(const Edge &e: edges)
        printEdge(e);
    cout << "#k: " << edges.size() << endl;
}
