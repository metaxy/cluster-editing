#include "mgraph.h"
#include <fstream>
#include <sstream>

MGraph::MGraph(int nodeCount) : m_nodeCount(nodeCount)
{
    m_matrix = new NodeT*[m_nodeCount];

    //init vector where is saved whether the node is deleted,
    //when value is -1, this means it is not deleted
    //when value is >= 0, this is node it was merged with
    this->m_deleted = new NodeT[m_nodeCount];

    memset(this->m_deleted, -1, m_nodeCount * sizeof(NodeT));

    //init main matrix
    for (int i=0; i<m_nodeCount; i++) {
       this->m_matrix[i] = new NodeT[m_nodeCount];
       memset(this->m_matrix[i], -1, m_nodeCount * sizeof(NodeT));
    }
}
MGraph::MGraph(Graph input) : MGraph(input.nNodes)
{
    m_input = input;
}
MGraph::MGraph(MGraph *copy)
{
    m_nodeCount = copy->m_nodeCount;
    m_input = copy->m_input;
    m_matrix = new NodeT*[m_nodeCount];
    m_deleted = new NodeT[m_nodeCount];
    for(int i = 0; i < m_nodeCount; i++) {
        m_deleted[i] = copy->m_deleted[i];
        m_matrix[i] = new NodeT[m_nodeCount];
        for(int j = 0; j < m_nodeCount; j++) {
            m_matrix[i][j] = copy->m_matrix[i][j];
        }
    }
}

MGraph::~MGraph()
{
    delete[] m_deleted;
    for(int i = 0; i < m_nodeCount; i++) {
        delete [] m_matrix[i];
    }
}
void MGraph::addEdge(const Edge &e)
{
   m_matrix[e.first][e.second] = 1;
   m_matrix[e.second][e.first] = 1;
}
int MGraph::absolut(NodeT u, NodeT v) const
{
    return abs(m_matrix[u][v]);
}
int MGraph::absolut(const Edge &e) const
{
    return absolut(e.first,e.second);
}
bool MGraph::isDeleted(NodeT x) const
{
    return m_deleted[x] != -1;
}
bool MGraph::connected(NodeT x, NodeT y) const
{
    return m_matrix[x][y] > 0;
}
bool MGraph::connected(const Edge &e) const
{
    return connected(e.first,e.second);
}
void MGraph::setWeight(Edge e, int weight)
{
    m_matrix[e.first][e.second] = weight;
    m_matrix[e.second][e.first] = weight;
}
void MGraph::flip(const Edge &e)
{
    m_matrix[e.first][e.second] = -m_matrix[e.first][e.second];
    m_matrix[e.second][e.first] = -m_matrix[e.second][e.first];
}

int MGraph::getWeight(Edge e) const
{
    return m_matrix[e.first][e.second];
}
int MGraph::getWeight(NodeT x, NodeT y) const
{
    return m_matrix[x][y];
}
int MGraph::merge(const Edge e)
{
    const NodeT u = e.first;
    const NodeT v = e.second;

    int depth = 0;

    for(int i = 0; i< m_nodeCount; i++) {
        if(i != v && i != u && !isDeleted(i)) {
            if(sgn(m_matrix[i][u]) != sgn(m_matrix[i][v])) {
                depth += min(abs(m_matrix[i][u]), abs(m_matrix[i][v]));
            }
            m_matrix[i][u] += m_matrix[i][v];
            m_matrix[u][i] = m_matrix[i][u];
        }
    }

    m_deleted[v] = u;//v is deleted
    return depth;
}
int MGraph::dismerge(const Edge e)
{
    const NodeT u = e.first;
    const NodeT v = e.second;
    int depth = 0;

    for(int i = 0; i< m_nodeCount; i++) {
        if(i != v && i != u && !isDeleted(i)) {
            m_matrix[i][u] -= m_matrix[i][v];
            m_matrix[u][i] = m_matrix[i][u];
            if(sgn(m_matrix[i][u]) != sgn(m_matrix[i][v])) {
                depth += min(abs(m_matrix[i][u]), abs(m_matrix[i][v]));
            }
        }
    }

    //node v is not deleted
    m_deleted[v] = -1;
    return depth;
}

vector<NodeT> MGraph::nodes() const
{
    vector<NodeT> list;
    for(int i = 0; i < m_nodeCount; i++) {
        if(!isDeleted(i)) {
            list.push_back(i);
        }
    }
    return list;
}

set<NodeT> MGraph::nodesSet() const
{
    set<NodeT> list;
    for(int i = 0; i < m_nodeCount; i++) {
        if(!isDeleted(i)) {
            list.insert(i);
        }
    }
    return list;
}


vector<Edge> MGraph::edges() const
{
    vector<Edge> list;
    for(int i = 0; i < m_nodeCount; i++) {
        if(isDeleted(i)) continue;
        for(int j = i+1; j < m_nodeCount; j++) {
            if(isDeleted(i)) continue;
            list.push_back(Edge(i,j));
        }
    }
 }

Model MGraph::createModel() const
{
    Model model;
    for(int i = 0; i < m_nodeCount; i++) {
       for(int j = i+1; j < m_nodeCount; j++) {
           if(!isDeleted(i) && !isDeleted(j)) {
               model[Edge(i,j)] = getWeight(i,j);
           }
       }
   }
   return model;
}
Edge MGraph::normalize(Edge e)
{
    if(e.first > e.second) return Edge(e.second, e.first);
    return e;
}

Edge MGraph::edge(NodeT x, NodeT y)
{
    if(x > y) return Edge(y,x);
    return Edge(x,y);
}
vector<P3> MGraph::findAllP3s() const
{
    de("find all p3s");
    vector<P3> ret;
    for(int i = 0; i < m_nodeCount; i++) {
        if(isDeleted(i)) continue;

        for(int j = 0; j < m_nodeCount; j++) {
            if(isDeleted(j) || i == j || !connected(i,j)) continue;

            for(int k = 0; k < m_nodeCount; k++) {
                if(isDeleted(k)) continue;

                if(j != k && i != k && connected(j,k) && !connected(i,k)) {
                    ret.push_back(P3(i,j,k));
                }
            }

        }
    }
    return ret;
}
int MGraph::nodeCount() const
{
    return m_nodeCount;
}

vector<Edge> MGraph::difference(MGraph *other)
{
    vector<Edge> list;
    for(int i = 0; i < m_nodeCount; i++) {
        if(isDeleted(i)) continue;
        for(int j = i+1; j < m_nodeCount; j++) {
            if(isDeleted(j)) continue;
            if(other->m_matrix[i][j] - m_matrix[i][j] != 0)
                list.push_back(Edge(i,j));
        }
    }
    return list;
}

void MGraph::restoreMerges()
{

    int count = 0;
    while(count <= m_nodeCount) {
        for(int i = 0; i< m_nodeCount; i++) {
            if(isDeleted(i)) {
                NodeT mergeTarget = m_deleted[i];
                if(!isDeleted(mergeTarget)) {
                    for(int t=0; t < m_nodeCount; t++) {
                        if(t == mergeTarget) {
                            m_matrix[i][t] = 1;
                            m_matrix[t][i] = 1;
                        } else {
                            m_matrix[i][t] = connected(mergeTarget,t) ? 1 : -1;
                            m_matrix[t][i] = connected(mergeTarget,t) ? 1 : -1;
                        }
                    }
                    m_deleted[i] = -1;//i is not anymore deleted
                } else {
                    count = -1;
                }
            }
            count++;
        }
    }
}

void MGraph::normalize()
{
   for(int i=0; i< m_nodeCount; i++) {
       for(int j=0; j<m_nodeCount; j++) {
           m_matrix[i][j] = connected(i,j) ? 1 : -1;
       }
   }

}
set<NodeT> MGraph::neighborhood(NodeT node) const
{
    set<NodeT> ret;
    for(int i = 0; i< m_nodeCount; i++) {
        if(isDeleted(i) || node == i) continue;
        if(connected(node, i)) ret.insert(i);
    }
    return ret;
}

void MGraph::printMatrix() const
{
    #ifdef _DEBUG
    Common::printMatrix(m_matrix, m_nodeCount);
    #endif
}

string MGraph::printGraph(P3 p3)
{
    #ifdef _DEBUG
    string ret = "digraph G {\n";
    ret += "edge [dir=none]\n";
    const NodeT a = get<0>(p3);
    const NodeT b = get<1>(p3);
    const NodeT c = get<2>(p3);
    for(int x = 0; x < m_nodeCount; x++) {
        if(isDeleted(x))
            continue;

        for(int y = x+1; y < m_nodeCount; y++) {
            if(!isDeleted(y) && connected(x,y)) {
                ret += "" +  m_input.getNodeByInt(x) + " -> " +  m_input.getNodeByInt(y) + " [label=\""+std::to_string(m_matrix[x][y]) + "\"";

                if((x == a && y == c) || (x == c && y == a) ||
                   (x == b && y == c) || (x == c && y == b) ||
                   (x == a && y == b) || (x == b && y == a)
                   ) {
                    ret += ",color=red";
                }
                ret += "];\n";
            }
        }
    }
    ret += "\n}\n";
    return ret;
    #endif
}


void MGraph::writeGraph(string fileName, P3 p3)
{
    #ifdef _DEBUG
    std::ofstream fout(fileName+".dot");
    fout << printGraph(p3);
    fout.close();
    std::stringstream stream;
    stream << "dot -Tpng \"" << fileName << ".dot\" -o \"" << fileName << ".png\"";
    system(stream.str().c_str());
    //remove((fileName+".dot").c_str());
    #endif
}
