// C++
/**
* @file SimpleExporter.cpp
* @authors
*   Francisco Eduardo Fontenele - 15452569
*   Vinicius Botte - 15522900
*
* \brief Implementação da exportação simples para arquivos texto.
*
* \details Serializa vértices e arestas únicas do grafo, e MST, em formatos fáceis
*          de consumir por Python sem dependências de Graphviz.
*
* \pre Arquivos de saída devem ser graváveis.
* \post Arquivos `graph_vertices.txt`, `graph_edges.txt` e `mst_edges.txt` gerados conforme chamado.
*/
#include "SimpleExporter.h"
#include <fstream>
using namespace std;

static bool writeLines(const string& path, const vector<string>& lines) {
    ofstream out(path);
    if (!out) return false;
    for (const auto& l : lines) out << l << "\n";
    return true;
}

bool SimpleExporter::exportVertices(const Graph& g, const string& verticesPath) {
    vector<string> vs = g.getAllVertices();
    return writeLines(verticesPath, vs);
}

bool SimpleExporter::exportEdges(const Graph& g, const string& edgesPath) {
    ofstream out(edgesPath);
    if (!out) return false;
    for (const auto& e : g.getEdgesUnique()) {
        out << e.u() << " " << e.v() << " " << e.weight() << "\n";
    }
    return true;
}

bool SimpleExporter::exportMst(const vector<Edge>& mst, const string& mstPath) {
    ofstream out(mstPath);
    if (!out) return false;
    for (const auto& e : mst) {
        out << e.u() << " " << e.v() << " " << e.weight() << "\n";
    }
    return true;
}
