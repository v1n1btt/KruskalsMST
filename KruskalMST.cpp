/**
* @file KruskalMST.cpp
* @authors
*   Francisco Eduardo Fontenele - 15452569
*   Vinicius Botte - 15522900
*
* \brief Implementação de Kruskal com Union-Find para MST.
*
* \details Ordena arestas por peso e seleciona as que não criam ciclo.
*
* \pre O grafo deve fornecer arestas únicas e conjunto de vértices.
* \post Vetor de MST contém arestas da árvore geradora mínima; custo total é atualizado.
*/

#include "KruskalMST.h"
#include <algorithm>
using namespace std;

void KruskalMST::makeSet(const vector<string>& vertices) {
    parent.clear();
    rank.clear();
    for (const auto& v : vertices) {
        parent[v] = v;
        rank[v] = 0;
    }
}

string KruskalMST::findSet(const string& v) {
    auto it = parent.find(v);
    if (it == parent.end()) return v;
    if (it->second == v) return v;
    it->second = findSet(it->second);
    return it->second;
}

void KruskalMST::unionSets(const string& a, const string& b) {
    string ra = findSet(a);
    string rb = findSet(b);
    if (ra == rb) return;
    if (rank[ra] < rank[rb]) {
        parent[ra] = rb;
    } else if (rank[ra] > rank[rb]) {
        parent[rb] = ra;
    } else {
        parent[rb] = ra;
        rank[ra] += 1;
    }
}

void KruskalMST::compute(const Graph& g, vector<Edge>& outMst, int& outTotalCost) {
    outMst.clear();
    outTotalCost = 0;

    vector<Edge> edges = g.getEdgesUnique();
    sort(edges.begin(), edges.end());

    vector<string> vertices = g.getAllVertices();
    sort(vertices.begin(), vertices.end());
    vertices.erase(unique(vertices.begin(), vertices.end()), vertices.end());

    makeSet(vertices);

    for (const auto& e : edges) {
        string ru = findSet(e.u());
        string rv = findSet(e.v());

        if (ru != rv) {
            outMst.push_back(e);
            outTotalCost += e.weight();
            unionSets(ru, rv);
        }
    }
}
