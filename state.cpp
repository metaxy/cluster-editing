#include "state.h"
#include <cassert>
#include <sstream>

using namespace std;

State::State(Graph g) : m_graph(g)
{
    de("State::State(Graph)");
    m_k = 0;
    m_recsteps = 0;
    m_input = g;
    for (GEdge *e = g.firstEdge; e != NULL; e = e->next) {
       m_graph.addEdge(e->toEdge());
    }

}
State::~State()
{
}

void State::solveFull()
{
    MGraph current(m_graph);
    if(m_flag_reduceZero) {
        reduceZero(&current);
    }

    MGraph solved = this->solve(current);
    solved.restoreMerges();
    solved.normalize();
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
void State::setConfig(map<string, string> config)
{
    if(config["reduceAll"] == "1") {
        m_flag_reduceZero = true;
    }
    if(config["reduceZero"] == "1") {
        m_flag_reduceZero = true;
    }
}
