/**
* @file Graph.cpp
 * @authors
 *   Francisco Eduardo Fontenele - 15452569
 *   Vinicius Botte - 15522900
 *
 * AED II - Trabalho 2
 */

#include "Graph.h"
#include <iostream>

using namespace std;

/**
 * @brief Constrói grafo a partir do arquivo
 * @param filename_ Caminho do arquivo.
 */
Graph::Graph(const string& filename_) {
    adjList = readGraph(filename_);
}

/**
 * @brief Lê uma string do arquivo binário.
 * @return String lida do arquivo.
 */
string Graph::readString(ifstream& in) {
    int len;
    in.read(reinterpret_cast<char*>(&len), sizeof(int));
    string s(len, '\0');
    in.read(&s[0], len);
    return s;
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

    map<Key, list<pair<Neighbor, Cost>>> adjList;

    for(int i = 0; i < n; i++) {
        string key = readString(in);

        // Lê grau do vértice
        int degree;
        in.read(reinterpret_cast<char*>(&degree), sizeof(int));

        list<pair<Neighbor, Cost>> neighbors;

        // Lê vértices adjacentes e seus pesos
        for(int j = 0; j < degree; j++){
            string nb = readString(in);
            int cost;

            in.read(reinterpret_cast<char*>(&cost), sizeof(int));
            neighbors.push_back({nb, cost});
        }

        adjList[key] = move(neighbors);
    }

    return adjList;
}

/**
 * @brief Escreve uma string no arquivo binário.
 * @param s String a ser escrita.
 */
void Graph::writeString(ofstream& out, const string& s) {
    int len = s.size();
    out.write(reinterpret_cast<const char*>(&len), sizeof(int));
    out.write(s.data(), len);
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
        writeString(out, key);

        // Escreve grau do vértice
        int degree = neighbors.size();
        out.write(reinterpret_cast<const char*>(&degree), sizeof(int));

        // Escreve vértices adjacentes com pesos
        for(const auto& [nb, cost] : neighbors){
            writeString(out, nb);
            out.write(reinterpret_cast<const char*>(&cost), sizeof(int));
        }
    }
}

/**
 * @brief Exibe o grafo completo.
 */
void Graph::displayGraph() {
    cout << "n = " << adjList.size() << endl;
    cout << "Lista de adjacência para o grafo:" << endl;
    for (auto& i : adjList) {
        cout << i.first << " -> ";
        for(auto& j : i.second) {
            cout << j.first << "(" << j.second << ")" << " ";
        }
        cout << endl;
    }
}

/**
 * @brief Insere novo vértice no grafo
 * @param key Chave do vértice
 */
void Graph::insertVertex(string key) {
    adjList[key] = {};
}

/**
 * @brief Insere nova aresta no grafo.
 * @param keyA Chave do primeiro vértice a ser conectado
 * @param keyB Chave do segundo vértice a ser conectado
 * @param cost Custo da aresta
 */
void Graph::insertEdge(string keyA, string keyB, int cost) {
    adjList[keyA].push_back({keyB, cost});
}