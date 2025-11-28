// C++
/**
* @file SimpleExporter.h
* @authors
*   Francisco Eduardo Fontenele - 15452569
*   Vinicius Botte - 15522900
*
* \brief Exporta vértices e arestas do grafo/MST para arquivos texto simples.
*
* \details Gera dois arquivos: `graph_vertices.txt` (um vértice por linha)
*          e `graph_edges.txt` (linhas "u v w"). Opcionalmente grava `mst_edges.txt`.
*
* \pre Caminhos devem ser graváveis; o grafo deve estar carregado.
* \post Arquivos texto gerados para consumo por script Python.
*/
#ifndef SIMPLE_EXPORTER_H
#define SIMPLE_EXPORTER_H

#include <string>
#include <vector>
#include "Graph.h"
#include "Edge.h"
using namespace std;

class SimpleExporter {
public:
    /**
     * \brief Exporta vértices para arquivo texto (um por linha).
     */
    static bool exportVertices(const Graph& g, const string& verticesPath);

    /**
     * \brief Exporta arestas únicas para arquivo texto ("u v w" por linha).
     */
    static bool exportEdges(const Graph& g, const string& edgesPath);

    /**
     * \brief Exporta arestas da MST para arquivo texto ("u v w" por linha).
     */
    static bool exportMst(const vector<Edge>& mst, const string& mstPath);
};
#endif
