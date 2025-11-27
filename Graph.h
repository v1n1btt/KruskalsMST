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
using Key = string;
using Neighbor = string;
using Cost = int;

class Graph {
    private:
        std::string filename;
        map<Key, list<pair<Neighbor, Cost>>> adjList;

        /**
         * @brief Lê uma string do arquivo binário.
         * @return String lida do arquivo.
         */
        string readString(ifstream& in);

        /**
         * @brief Lê o grafo do arquivo.
         * @return Grafo carregado do arquivo.
         */
        map<Key, list<pair<Neighbor, Cost>>> readGraph();

        /**
         * @brief Escreve uma string no arquivo binário.
         * @param s String a ser escrita.
         */
        void writeString(ofstream& out, const string& s);

        /**
         * @brief Persiste o grafo no arquivo.
         */
        void writeGraph();

    public:
        /**
         * @brief Constrói grafo a partir do arquivo
         * @param filename_ Caminho do arquivo.
         */
        explicit Graph(const string& filename_);

        /**
         * @brief Salva as alterações no arquivo binário.
         */
        ~Graph();

        /**
         * @brief Exibe o grafo completo.
         */
        void displayGraph();

        /**
         * @brief Insere novo vértice no grafo
         * @param key Chave do vértice
         */
        void insertVertex(string key);

        /**
         * @brief Insere nova aresta no grafo.
         * @param keyA Chave do primeiro vértice a ser conectado
         * @param keyB Chave do segundo vértice a ser conectado
         * @param cost Custo da aresta
         */
        void insertEdge(string keyA, string keyB, int cost);
};

#endif