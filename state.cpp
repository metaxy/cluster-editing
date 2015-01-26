#include "state.h"
#include <cassert>
#include <sstream>

using namespace std;

State::State(Graph g) : m_graph(g), m_k(0)
{
    de("State::State(Graph)");
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
    if(m_flag_reduce2K) {
        reduce2K(&current);
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
void State::reduce2K(MGraph *graph)
{
    for(NodeT u : graph->nodes()) {
        set<NodeT> n = graph->neighborhood(u);
        if(2*graph->costMakingClique(u) + graph->costCutting(u) < n.size()) {
            for(NodeT a : n) {
                for(NodeT b : n) {
                    if(a == b || graph->isDeleted(a) || graph->isDeleted(b)) continue;
                    m_k += graph->merge(Edge(a,b));
                }
            }
        }
    }
}

void State::setConfig(map<string, string> config)
{
    if(config["reduceAll"] == "1") {
        m_flag_reduceZero = true;
        m_flag_reduce2K = true;
    }
    if(config["reduceZero"] == "1") {
        m_flag_reduceZero = true;
    }
    if(config["reduce2K"] == "1") {
        m_flag_reduce2K = true;
    }
}
