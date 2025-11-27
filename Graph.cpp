/**
* @file Graph.cpp
 * @authors
 *   Francisco Eduardo Fontenele - 15452569
 *   Vinicius Botte - 15522900
 *
 * AED II - Trabalho 2
 */

#include "Graph.h"

using namespace std;

/**
 * @brief Constrói grafo a partir do arquivo
 * @param filename_ Caminho do arquivo.
 */
Graph::Graph(const string& filename_) {
    adjList = readGraph(filename_);
}

/**
 * @brief Lê o grafo do arquivo.
 * @param filename_ Caminho do arquivo.
 * @return Grafo carregado do arquivo.
 */
map<Key, list<pair<Neighbor, Cost>>> Graph::readGraph(const string& filename_) {
    ifstream in(filename, std::ios::binary);
    if (!in) return {};

    // Lê número de vértices
    int n;
    in.read(reinterpret_cast<char*>(&n), sizeof(int));

    map<Key, list<pair<Neighbor, Cost>>> graph;

    for(int i = 0; i < n; i++) {
        int key;
        in.read(reinterpret_cast<char*>(&key), sizeof(int));

        // Lê grau do vértice
        int degree;
        in.read(reinterpret_cast<char*>(&degree), sizeof(int));

        list<pair<Neighbor, Cost>> neighbors;

        // Lê vértices adjacentes e seus pesos
        for(int j = 0; j < degree; j++){
            int nb;
            int weight;

            in.read(reinterpret_cast<char*>(&nb), sizeof(int));
            in.read(reinterpret_cast<char*>(&weight), sizeof(int));
            neighbors.push_back({nb, weight});
        }

        graph[key] = move(neighbors);
    }

    return graph;
}

/**
 * @brief Persiste o grafo no arquivo.
 * @param filename_ Caminho do arquivo.
 */
void Graph::writeGraph(const string& filename_) {
    ofstream out(filename, std::ios::binary);
    if (!out) return;

    // Escreve número de vértices
    int n = adjList.size();
    out.write(reinterpret_cast<const char*>(&n), sizeof(int));

    for(const auto& [key, neighbors] : adjList){
        out.write(reinterpret_cast<const char*>(&key), sizeof(int));

        // Escreve grau do vértice
        int degree = neighbors.size();
        out.write(reinterpret_cast<const char*>(&degree), sizeof(int));

        // Escreve vértices adjacentes com pesos
        for(const auto& [nb, weight] : neighbors){
            out.write(reinterpret_cast<const char*>(&nb), sizeof(int));
            out.write(reinterpret_cast<const char*>(&weight), sizeof(int));
        }
    }
}