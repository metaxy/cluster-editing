#include "state.h"
#include <cassert>
#include <sstream>

using namespace std;

State::State(Graph g)
{
    m_k = 0;
    MGraph a(g);
    m_graph = a;
    m_recsteps = 0;
    
    for (GEdge *e = g.firstEdge; e != NULL; e = e->next) {
       m_graph.addEdge(e->toEdge());
    }

}
State::~State()
{
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
void State::solveFull()
{
    MGraph current(&m_graph);
    MGraph solved = this->solve(current);
    auto edges = m_graph.difference(&solved);
    cout << "#k: " << edges.size() << endl;
    printEdges(edges);
}

void State::printEdge(const Edge &e)
{
    cout << m_input.getNodeByInt(e.first) << " " << m_input.getNodeByInt(e.second) << endl;
}

void State::printEdges(vector<Edge> edges)
{
    for(const Edge &e: edges)
        printEdge(e);
}

void State::reduceZero(MGraph *graph)
{
    for(Edge e : graph->connectedEdges()) {
        if(graph->mergeCost(e) == 0)
            graph->merge(e);
    }
}
