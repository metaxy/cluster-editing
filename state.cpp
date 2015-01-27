#include "state.h"
#include <cassert>
#include <sstream>
#include "randomize.h"
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

void State::solveFull(int count)
{
    MGraph current(m_graph);
    if(m_flag_reduceZero) {
        reduceZero(&current);
    }
    if(m_flag_reduce2K) {
        reduce2K(&current);
    }
    vector<Edge> bestEdges;
    int bestSize = -1;
    for(int i = 0; i< count; i++) {
        MGraph solved = this->solve(current);
        solved.restoreMerges();
        solved.normalize();
        vector<Edge> edges = m_graph.difference(&solved);
        if(edges.size() < bestSize) {
            bestEdges = edges;
            bestSize = edges.size();
        }
    }
    cout << "#k: " << bestSize << endl;
    printEdges(bestEdges);
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
    if(config["reduceAll"] == "true") {
        m_flag_reduceZero = true;
        m_flag_reduce2K = true;
    } else {
        m_flag_reduceZero = false;
        m_flag_reduce2K = false;
    }
    if(config["reduceZero"] == "true") {
        m_flag_reduceZero = true;
    } else {
        m_flag_reduceZero = false;
    }
    if(config["reduce2K"] == "true") {
        m_flag_reduce2K = true;
    } else {
        m_flag_reduce2K = false;
    }
    m_config = config;
    parseConfig();
    m_seed = getInt("seed", 1);
}
void State::parseConfig()
{

}

double State::getDouble(const string &name, double def)
{
    auto iter = m_config.find(name);
    if(iter != m_config.end()) {
        std::istringstream i(iter->second);
        double x;
        if (!(i >> x))
            return def;
        return x;
    }
    return def;
}
int State::getInt(const string &name, int def)
{
    auto iter = m_config.find(name);
    if(iter != m_config.end()) {
        std::istringstream i(iter->second);
        int x;
        if (!(i >> x))
            return def;
        return x;
    }
    return def;
}

MGraph State::solveSurely(MGraph graph)
{
    Randomize r;
    MGraph res(graph);
    res.clear();

    set<NodeT> nodes = graph.nodesSet();
    while(!nodes.empty()) {
        NodeT u = r.randomElement(nodes);
        set<NodeT> cluster = graph.closedNeighborhood(u);
        cluster = set_intersect(cluster, nodes);
        for(NodeT v : cluster) {
            for(NodeT w : cluster) {
                if(v == w) continue;
                res.setWeight(Edge(v,w), 1);
            }
            nodes.erase(v);
        }
    }
    return res;
}
