#ifndef MGRAPH_H
#define MGRAPH_H
#include "common.h"
#include <vector>
#include <set>
class MGraph
{
public:
    MGraph();
    MGraph(int nodeCount);
    MGraph(Graph input);
    MGraph(MGraph *copy);
    MGraph(const MGraph &copy);
    ~MGraph();
    void addEdge(const Edge &e);
    bool connected(const Edge &e) const;
    int absolut(const Edge &e) const;
    bool isDeleted(NodeT x) const;
    void flip(const Edge &e);
    void setWeight(Edge e, int weight);
    int getWeight(Edge e) const;
    void clear();

    int merge(Edge e);
    int dismerge(Edge e);

    vector<NodeT> nodes() const;
    set<NodeT> nodesSet() const;
    vector<Edge> edges() const;
    set<NodeT> neighborhood(NodeT node) const;
    Model createModel() const;

    vector<P3> findAllP3s() const;

    static Edge normalize(Edge e);
    static Edge edge(NodeT x, NodeT y);

    int nodeCount() const;

    vector<Edge> difference(MGraph *other);

    void restoreMerges();
    void normalize();

    void printMatrix() const;
    string printGraph(P3 p3 = P3(0,0,0));
    void writeGraph(string fileName, P3 p3 = P3(0,0,0));

    int mergeCost(NodeT u, NodeT v) const;
    int mergeCost(Edge e) const;
    vector<Edge> connectedEdges() const;

    bool connected(NodeT x, NodeT y) const;

    bool isP3(P3 p3) const;
    P3 findNextBestP3(int minCost=0) const;
    P3 findP3() const;
    int costCutting(NodeT node);
    int costMakingClique(NodeT node);
    set<NodeT> closedNeighborhood(NodeT node) const;
    bool hasSameNeighbours(NodeT u, NodeT v);
    set<NodeT> costlyNeighborhood(NodeT node, int maxCost) const;
    set<NodeT> closedCostlyNeighborhood(NodeT node, int maxCost) const;
    bool fast2K(NodeT node, float c, float cu, float size, float diff) const;
   private:
    int m_nodeCount;
    Graph m_input;
    int absolut(NodeT u, NodeT v) const;
    int getWeight(NodeT x, NodeT y) const;
    
    NodeT **m_matrix = nullptr;
    NodeT *m_deleted = nullptr;

};

#endif // MGRAPH_H
