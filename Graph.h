/**
* @file Graph.h
 * @authors
 *   Francisco Eduardo Fontenele - 15452569
 *   Vinicius Botte - 15522900
 *
 * AED II - Trabalho 2
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <fstream>
#include <map>
#include <list>
#include <string>

using namespace std;
using Key = int;
using Neighbor = int;
using Cost = int;

class Graph {
    private:
        std::string filename;
        map<Key, list<pair<Neighbor, Cost>>> adjList;

        /**
         * @brief Lê o grafo do arquivo.
         * @param filename_ Caminho do arquivo.
         * @return Grafo carregado do arquivo.
         */
        map<Key, list<pair<Neighbor, Cost>>> readGraph(const string& filename_);

        /**
         * @brief Persiste o grafo no arquivo.
         * @param filename_ Caminho do arquivo.
         */
        void writeGraph(const string& filename_);

    public:
        /**
         * @brief Constrói grafo a partir do arquivo
         * @param filename_ Caminho do arquivo.
         */
        Graph(const string& filename_);

};

#endif