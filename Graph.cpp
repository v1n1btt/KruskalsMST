/**
* @file Graph.cpp
* @authors
*   Francisco Eduardo Fontenele - 15452569
*   Vinicius Botte - 15522900
*
* \brief Implementação da classe Graph para redes de computadores.
*
* \details Oferece CRUD de vértices/arestas, busca, impressão, importação .txt e persistência binária.
*
* \pre O arquivo binário deve ser acessível; chaves devem ser estáveis ao longo do uso.
* \post O estado do grafo é refletido no arquivo quando o objeto é destruído.
*/

#include "Graph.h"
#include "NetworkTopologyParser.h"
#include <iostream>
#include <fstream>
#include <utility>
using namespace std;

static bool containsNeighbor(const list<pair<string,int>>& lst, const string& k, int* cost) {
    for (const auto& p : lst) {
        if (p.first == k) {
            if (cost) *cost = p.second;
            return true;
        }
    }
    return false;
}

string Graph::readString(ifstream& in) {
    int len = 0;
    in.read(reinterpret_cast<char*>(&len), sizeof(int));
    if (len <= 0) return string();
    string s(static_cast<size_t>(len), '\0');
    in.read(&s[0], len);
    return s;
}

map<string, list<pair<string, int>>> Graph::readGraph() {
    ifstream in(filename, ios::binary);
    if (!in) return {};
    int magic = 0, version = 0, n = 0;
    in.read(reinterpret_cast<char*>(&magic), sizeof(int));
    in.read(reinterpret_cast<char*>(&version), sizeof(int));
    if (!in || magic != MAGIC) return {};
    if (version != VERSION) return {};
    in.read(reinterpret_cast<char*>(&n), sizeof(int));
    map<string, list<pair<string, int>>> result;
    for (int i = 0; i < n; ++i) {
        string key = readString(in);
        int degree = 0;
        in.read(reinterpret_cast<char*>(&degree), sizeof(int));
        list<pair<string,int>> neighbors;
        for (int j = 0; j < degree; ++j) {
            string nb = readString(in);
            int cost = 0;
            in.read(reinterpret_cast<char*>(&cost), sizeof(int));
            neighbors.emplace_back(nb, cost);
        }
        result[key] = std::move(neighbors);
    }
    return result;
}

void Graph::writeString(ofstream& out, const string& s) {
    int len = static_cast<int>(s.size());
    out.write(reinterpret_cast<const char*>(&len), sizeof(int));
    if (len > 0) out.write(s.data(), len);
}

void Graph::writeGraph() const {
    ofstream out(filename, ios::binary | ios::trunc);
    if (!out) return;
    out.write(reinterpret_cast<const char*>(&MAGIC), sizeof(int));
    out.write(reinterpret_cast<const char*>(&VERSION), sizeof(int));
    int n = static_cast<int>(adjList.size());
    out.write(reinterpret_cast<const char*>(&n), sizeof(int));
    for (const auto& kv : adjList) {
        writeString(out, kv.first);
        int degree = static_cast<int>(kv.second.size());
        out.write(reinterpret_cast<const char*>(&degree), sizeof(int));
        for (const auto& nb : kv.second) {
            writeString(out, nb.first);
            out.write(reinterpret_cast<const char*>(&nb.second), sizeof(int));
        }
    }
}

Graph::Graph(const string& filename_) : filename(filename_) {
    adjList = readGraph();
    if (adjList.empty()) writeGraph();
}

Graph::~Graph() {
    writeGraph();
}

void Graph::displayGraph() const {
    cout << "\n====================\n";
    cout << "Estado atual do grafo\n";
    cout << "====================\n";
    cout << "n = " << adjList.size() << "\n";
    cout << "Lista de adjacencia:\n";
    for (const auto& kv : adjList) {
        cout << "  " << kv.first << " -> ";
        for (const auto& p : kv.second) {
            cout << p.first << "(" << p.second << ") ";
        }
        cout << "\n";
    }
    cout << "\n";
}

bool Graph::hasVertex(const string& key) const {
    return adjList.find(key) != adjList.end();
}

bool Graph::hasEdge(const string& a, const string& b, int* outCost) const {
    auto it = adjList.find(a);
    if (it == adjList.end()) return false;
    return containsNeighbor(it->second, b, outCost);
}

void Graph::insertVertex(const string& key) {
    if (!hasVertex(key)) adjList[key] = {};
}

void Graph::insertEdge(const string& a, const string& b, int cost) {
    if (!hasVertex(a) || !hasVertex(b)) return;
    int tmp;
    if (!hasEdge(a, b, &tmp)) {
        adjList[a].emplace_back(b, cost);
        adjList[b].emplace_back(a, cost);
    }
}

void Graph::deleteUndirectedEdge(const string& a, const string& b) {
    auto ita = adjList.find(a);
    if (ita != adjList.end()) {
        ita->second.remove_if([&](const pair<string,int>& p){ return p.first == b; });
    }
    auto itb = adjList.find(b);
    if (itb != adjList.end()) {
        itb->second.remove_if([&](const pair<string,int>& p){ return p.first == a; });
    }
}

void Graph::deleteVertex(const string& key) {
    adjList.erase(key);
    for (auto& kv : adjList) {
        kv.second.remove_if([&](const pair<string,int>& p){ return p.first == key; });
    }
}

vector<Edge> Graph::getEdgesUnique() const {
    vector<Edge> edges;
    for (const auto& kv : adjList) {
        const auto& u = kv.first;
        for (const auto& p : kv.second) {
            const auto& v = p.first;
            int w = p.second;
            if (u < v) edges.emplace_back(u, v, w);
        }
    }
    return edges;
}

void Graph::importFromTxt(const string& path) {
    vector<string> verts;
    vector<Edge> edges;
    NetworkTopologyParser::parse(path, verts, edges);
    for (const auto& v : verts) insertVertex(v);
    for (const auto& e : edges) insertEdge(e.u(), e.v(), e.weight());
}

void Graph::clearAndPersist() {
    adjList.clear();
    writeGraph();
}

vector<string> Graph::getAllVertices() const {
    vector<string> vs;
    vs.reserve(adjList.size());
    for (const auto& kv : adjList) vs.push_back(kv.first);
    return vs;
}
