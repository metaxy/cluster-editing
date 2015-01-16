#include "state.h"
#include "state_blp.h"
#include "state_blp_rel.h"
#include "state_lp_round.h"
#include "state_lp_round.h"
#include <cassert>
#include <sstream>

using namespace std;

State::State(Graph *g, int db)
{
	debug = db;
	m_graph = g;
	m_nodeCount = g->nNodes;
	m_k = 0;
	m_recsteps = 0;
	
	m_matrix = new NodeT*[m_nodeCount+2];
	m_matrix[0] = new NodeT[(m_nodeCount+2) * m_nodeCount];
	memset(m_matrix[0], -1, (m_nodeCount+2) * m_nodeCount * sizeof(int));
	for (NodeT i = 1; i<m_nodeCount+2; i++) m_matrix[i] = m_matrix[i-1] + m_nodeCount;
	
    for (GEdge *e = g->firstEdge; e != NULL; e = e->next) {
        m_matrix[e->u][e->v] = 1;
        m_matrix[e->v][e->u] = 1;
     }

	m_deleted = m_matrix[m_nodeCount];
    m_merged = m_matrix[m_nodeCount+1];
    m_tn = m_tnc = 0;
    
    m_nodeWeight = new int[m_nodeCount];
    for (int i=0; i<m_nodeCount; i++) m_nodeWeight[i] = 1;

    m_CC = new NodeT[m_nodeCount];

	m_matrix_copy = new NodeT[m_nodeCount * m_nodeCount];
	memcpy(m_matrix_copy, m_matrix[0], m_nodeCount * m_nodeCount * sizeof(NodeT));

    m_bestresult = new NodeT[(m_nodeCount+2) * m_nodeCount];
    memcpy(m_bestresult, m_matrix[0], (m_nodeCount+2) * m_nodeCount * sizeof(NodeT));
   
    m_nDivisions = 0;
    m_nCore = 0;
}

State::~State()
{
/*
	delete[] bestresult;
	delete[] CC;
	delete[] merged;
	delete[] NodeWeight;
	delete[] m_deleted;
	delete[] m_matrix_copy;
	delete[] m_matrix[0];
	delete[] m_matrix;
*/
}
int State::absolut(NodeT u, NodeT v) const
{
    return abs(m_matrix[u][v]);
}
bool State::isDeleted(NodeT x) const
{
    return m_deleted[x] != -1;
}
bool State::connected(NodeT x, NodeT y) const
{
    return m_matrix[x][y] > 0;
}
void State::setWeight(Edge e, int weight)
{
    m_matrix[e.first][e.second] = weight;
    m_matrix[e.second][e.first] = weight;
}
int State::getWeight(Edge e) const
{
    return m_matrix[e.first][e.second];
}

bool State::hasSameNeighbours(NodeT u, NodeT v)
{
	NodeT i=-1;
    while (++i < u) {
        if (!isDeleted(i) && connected(i,u) != connected(i, v))
            return false;
    }
    while (++i < v) {
        if (!isDeleted(i) && connected(u,i) != connected(i, v))
            return false;
    }
    while (++i < m_nodeCount) {
        if (!isDeleted(i) && connected(u,i) != connected(v, i))
            return false;
    }
	return true;
}

int State::mergeCost(NodeT u, NodeT v)
{
    int dk = 0;
    NodeT i = -1;
    while (++i < u)  {
        if (!isDeleted(i) && (connected(i,u) != connected(i, v))) {
            dk += min(absolut(i,u), absolut(i,v));
        }
    }
    while (++i < v) {
        if (!isDeleted(i) && (connected(u,i) != connected(i, v))) {
            dk += min(absolut(u,i), absolut(i,v));
        }
    }
    while (++i < m_nodeCount) {
        if (!isDeleted(i) && (connected(u,i) != connected(v, i))) {
            dk += min(absolut(u,i), absolut(v,i));
        }
    }
	return dk;
}

void State::merge(NodeT u, NodeT v)
{
	NodeT i=-1;
    while (++i < u) {
        if (!isDeleted(i)) {
            m_matrix[i][u] += m_matrix[i][v];
        }
    }
    while (++i < v) {
        if (!isDeleted(i)) {
            m_matrix[u][i] += m_matrix[i][v];
        }
    }
    while (++i < m_nodeCount) {
        if (!isDeleted(i)) {
            m_matrix[u][i] += m_matrix[v][i];
        }
    }
    m_deleted[v] = u; //v is merged with u
    m_nodeWeight[u] += m_nodeWeight[v];
    m_merged[m_tn] = v;
    m_tn ++;
}

void State::dismerge()
{
    NodeT v = m_merged[--m_tn];
    NodeT u = m_deleted[v];
    NodeT i = -1;
    while (++i<u) {
        if (!isDeleted(i)) {
            m_matrix[i][u] -= m_matrix[i][v];
        }
    }
    while (++i<v) {
        if (!isDeleted(i)) {
            m_matrix[u][i] -= m_matrix[i][v];
        }
    }
    while (++i<m_nodeCount) {
        if (!isDeleted(i)) {
            m_matrix[u][i] -= m_matrix[v][i];
        }
    }
    m_deleted[v] = -1; //v is not merged
    m_nodeWeight[u] -= m_nodeWeight[v];
}

bool State::mergeTwin()
{
    for (NodeT u=0; u<m_nodeCount; u++) {
        if (!isDeleted(u)) {
            for (NodeT v = u+1; v < m_nodeCount; v++) {
                if (!isDeleted(v) && connected(u, v) && hasSameNeighbours(u, v)) {
					merge(u, v);
					return true;
				}
            }
       }
    }
	return false;
}

int State::mergeTwins()
{
	while (mergeTwin());
	return 0;
}

bool State::core()
{
    for (NodeT u = 0; u < m_nodeCount; u++) {
        if (!isDeleted(u)) {
            if (core(u) >= 0)  {
                return true;
            }
        }
    }
	return false;
}

int State::core(NodeT u)
{
	NodeT v, w, x=0;
	int n=0, z, wu, wv;
	int beta, gamma, delta;
    memset(m_CC, 0, m_nodeCount * sizeof(NodeT));
	for (x=0; x<u; x++) if (!isDeleted(x) && connected(x, u)) break;
	v = x-1;
	while (++v < u)
        if (!isDeleted(v) && connected(v, u)) {m_CC[v] = 1; n += m_nodeWeight[v];}
	while (++v < m_nodeCount)
        if (!isDeleted(v) && connected(u, v)) {m_CC[v] = 1; n += m_nodeWeight[v];}
	if (n == 0) return -1;
    n += m_nodeWeight[u];
	v = x-1;
	delta = 0;
    while (++v < u) if (m_CC[v]) {
		w = -1;
		while (++w < v) if (!isDeleted(w)) {
			wu=m_matrix[w][u];
			wv=m_matrix[w][v];
			if ((wu>0) && (wv<=0)) {n+=wv; delta-=wv;}
			if ((wu<=0) && (wv>0)) {n-=wv;}
			if (n <= 0) return -1;
		}
		while (++w < u) if (!isDeleted(w)) {
			wu=m_matrix[w][u];
			wv=m_matrix[v][w];
			if ((wu>0) && (wv<=0)) {n+=wv; delta-=wv;}
			if ((wu<=0) && (wv>0)) {n-=wv;}
			if (n <= 0) return -1;
		}
		while (++w < m_nodeCount) if (!isDeleted(w)) {
			wu=m_matrix[u][w];
			wv=m_matrix[v][w];
			if ((wu>0) && (wv<=0)) {n+=wv; delta-=wv;}
			if ((wu<=0) && (wv>0)) {n-=wv;}
			if (n <= 0) return -1;
		}
	}
    while (++v < m_nodeCount) if (m_CC[v]) {
		w = -1;
		while (++w < u) if (!isDeleted(w)) {
			wu=m_matrix[w][u];
			wv=m_matrix[w][v];
			if ((wu>0) && (wv<=0)) {n+=wv; delta-=wv;}
			if ((wu<=0) && (wv>0)) {n-=wv;}
			if (n <= 0) return -1;
		}
		while (++w < v) if (!isDeleted(w)) {
			wu=m_matrix[u][w];
			wv=m_matrix[w][v];
			if ((wu>0) && (wv<=0)) {n+=wv; delta-=wv;}
			if ((wu<=0) && (wv>0)) {n-=wv;}
			if (n <= 0) return -1;
		}
		while (++w < m_nodeCount) if (!isDeleted(w)) {
			wu=m_matrix[u][w];
			wv=m_matrix[v][w];
			if ((wu>0) && (wv<=0)) {n+=wv; delta-=wv;}
			if ((wu<=0) && (wv>0)) {n-=wv;}
			if (n <= 0) return -1;
		}
	}
	delta /= 2;
    for (v=0; v<m_nodeCount; v++) if (!isDeleted(v)) if (!m_CC[v]) {
		w = -1;
		beta = gamma = 0;
        while (++w < v) if (m_CC[w]) {
			wv=m_matrix[w][v];
			if (wv<=0) beta -= wv;
			else gamma += wv;
		}
        while (++w < m_nodeCount) if (m_CC[w]) {
			wv=m_matrix[v][w];
			if (wv<=0) beta -= wv;
			else gamma += wv;
		}
		delta += min(beta, gamma);	
	}
	v = m_nodeCount;
	while (--v > u)
        if (m_CC[v]) {
            m_nCore ++;
			merge(u, v);
		}
	while (--v >= x)
        if (m_CC[v]) {
            m_nCore ++;
			merge(v, u);
			u = v;
		}
	return delta;
}

void State::normalize()
{
	NodeT u, v, i;
    while (m_tn) {
        v = m_merged[--m_tn];
        u = m_deleted[v];
		i=-1;
		while (++i<u) if (!isDeleted(i)) m_matrix[i][u] = m_matrix[i][v] = connected(i,u) ? 1 : -1;
		while (++i<v) if (!isDeleted(i)) m_matrix[u][i] = m_matrix[i][v] = connected(u,i) ? 1 : -1;
		while (++i<m_nodeCount) if (!isDeleted(i)) m_matrix[u][i] = m_matrix[v][i] = connected(u,i) ? 1 : -1;
	    m_matrix[u][v] = 1;
		m_deleted[v] = -1;
	}
}

int State::findBestP3(NodeT &a, NodeT &b, int needed)
{
    NodeT i=a, j, i0=a;
    int n=0, d, dk=-1;
	do {
        if (!isDeleted(i)) {
	        for (j = i+1; j < m_nodeCount; j++) {
		        int e = m_matrix[i][j];
				if (!isDeleted(j) && (e > 0))
				{
					d = mergeCost(i, j);
					if (d == 0) {
						a = i;
						b = j;
						return 2;
					}

					d += e;
					if (d > dk) {
						a = i;
						b = j;
						dk = d;
						if (d > needed) {
							return 3;
						}
					}
				}
	        }
        }
		if (++i == m_nodeCount) i = 0;
	} while (i != i0);
    if (dk >= 0) return 3;
	return 0;
}

void State::branch(NodeT start, int k)
{
    NodeT a=start, b, c, d;
    int dk, ck, fp, t0=m_tn;

	m_recsteps++;

    dk = 0.3*(m_k-k)+3; //if (k<80) dk = m_k-k;
	fp = findBestP3(a, b, dk);
	if (fp == 0) {
		m_k = k;
        memcpy(m_bestresult, m_matrix[0], (m_nodeCount+2) * m_nodeCount * sizeof(NodeT)); m_tnc = m_tn;
		if (debug) printState();
		return;
	}
	if (fp & 1) {
		dk = absolut(a,b);
		m_matrix[a][b] = -10000;	//remove edge(a, b)
		if (dk < m_k-k) branch(a, k+dk);
	    m_matrix[a][b] = dk;		//restore edge(a, b)
	}

    if (fp & 2) {
	    dk = mergeCost(a, b);
	    if (dk < m_k-k) {
	        merge(a, b);
            if (dk < m_k-k) {
                branch(a, k+dk);
            }
	    }
	}

    while (m_tn > t0) dismerge();
}

void State::solveZK(Graph *result)
{
	Graph g;
	NodeT x, y, z, *ZK = new NodeT[m_nodeCount];
	memset(ZK, -1, m_nodeCount * sizeof(NodeT));
	while (core());
	
	for (x=0; x<m_nodeCount; x++) if (!isDeleted(x)) if (ZK[x] == -1) {
        m_nDivisions ++;
		ZK[x] = x;
		for (y=x+1; y<m_nodeCount; y++) if (!isDeleted(y)) {
			if (ZK[y] == -1) {
				for (z=x; z<y; z++) if (!isDeleted(z))
					if (ZK[z]==x) if (connected(z,y)) {ZK[y]=x; break;}
				if (z==y) while (++z < m_nodeCount) if (ZK[z]==x) if (connected(y,z)) {ZK[y]=x; break;}
				if (ZK[y]==x) y = x;
			}
		}
	}

	for (x=0; x<m_nodeCount; x++) if(isDeleted(x)) ZK[x] = ZK[m_deleted[x]];
	
    while (m_tn) dismerge();
	for (x=0; x<m_nodeCount; x++)
		for (y=x+1; y<m_nodeCount; y++)
			if (connected(x, y) && (ZK[x] != ZK[y])) {
				m_matrix[x][y] = -1;
			}

	output(result);
	
	m_k = result->nEdges();
	if (debug) printState();
    m_nDivisions = 0;
	
    for (x=0; x<m_nodeCount; x++) {
        if (ZK[x] == x) {
            g.reset();
            for (y=x; y<m_nodeCount; y++) if (!isDeleted(y))
                if (ZK[y] == x)
                    for (z=y+1; z<m_nodeCount; z++) if (!isDeleted(z))
                        if(ZK[z] == x)
                            if (connected(y, z))
                                g.insertEdge(m_graph->getNodeByInt(y), m_graph->getNodeByInt(z));
            if (g.nEdges()) {
                auto *zk= this->clone(&g);
                zk->m_nDivisions = ++m_nDivisions;
                zk->solve(result);
                m_recsteps += zk->m_recsteps;
                m_nCore += zk->m_nCore;
                delete zk;
            }
        }
    }

	m_k = result->nEdges();

}

void State::output(Graph *result)
{
	for (NodeT x=0; x<m_nodeCount; x++)
		for (NodeT y=x+1; y<m_nodeCount; y++)
			if (connected(x, y) != (m_matrix_copy[x*m_nodeCount+y] > 0))
				if (result) result->insertEdge(m_graph->getNodeByInt(x), m_graph->getNodeByInt(y));
}

void State::printState()
{
	cout << "# k: " << m_k;
    cout << ", ZK: " << m_nDivisions;
    cout << ", C: " << m_nCore;
	cout << ", rs: " << m_recsteps;
	cout << endl;
}

bool State::isHeavy(NodeT i, NodeT j)
{
	int x, edgeSum=0, nonEdgeSum=0;
	NodeT h;
	for (h=0; h<i; h++) if ((x = m_matrix[h][i]) > 0) edgeSum += x; else nonEdgeSum -= x;			
	for (h++; h<j; h++) if ((x = m_matrix[i][h]) > 0) edgeSum += x; else nonEdgeSum -= x;			
	for (h++; h<m_nodeCount; h++) if ((x = m_matrix[i][h]) > 0) edgeSum += x; else nonEdgeSum -= x;			
	if (edgeSum + nonEdgeSum <= m_matrix[i][j]) {return true;}
	int sumReminder = edgeSum;
	nonEdgeSum = 0;
	edgeSum = 0;
	for (h=0; h<i; h++) if ((x = m_matrix[h][j]) > 0) edgeSum += x; else nonEdgeSum -= x;			
	for (h++; h<j; h++) if ((x = m_matrix[h][j]) > 0) edgeSum += x; else nonEdgeSum -= x;			
	for (h++; h<m_nodeCount; h++) if ((x = m_matrix[j][h]) > 0) edgeSum += x; else nonEdgeSum -= x;			
	if (edgeSum + nonEdgeSum <= m_matrix[i][j]) {return true;}
	if (edgeSum + sumReminder <= m_matrix[i][j]) {return true;}
	return false;
}

bool State::heavyEdge()
{
	for (NodeT i=0; i<m_nodeCount; i++) if(!isDeleted(i))
		for (NodeT j=i+1; j<m_nodeCount; j++) if(!isDeleted(j) && connected(i,j) && isHeavy(i,j)) {merge(i,j); return true;}
		return false;
}

bool State::heavyEdges()
{
	bool result = 0;
	while (heavyEdge()) result ++;
	return result;
}


State * State::clone(Graph *g)
{  
    if(dynamic_cast<StateBlp*> (this)) return new StateBlp(g);
    if(dynamic_cast<StateBlpRel*> (this)) return new StateBlpRel(g);
    if(dynamic_cast<StateLpRound*> (this)) return new StateLpRound(g);
    return nullptr;
}
vector<NodeT> State::nodes() const
{
    vector<NodeT> list;
    for(int i = 0; i < m_nodeCount; i++) {
        if(!isDeleted(i)) {
            list.push_back(i);
        }
    }
    return list;
}
list<P3> State::findAllP3s() const
{
    de("find all p3s");
    list<P3> ret;
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
Model State::createModel() const
{
    Model model;
    for(int i = 0; i < m_nodeCount; i++) {
       for(int j = i+1; j < m_nodeCount; j++) {
           if(!isDeleted(i) && !isDeleted(j)) {
               model[Edge(i,j)] = m_matrix[i][j];
           }
       }
   }
   return model;
}
