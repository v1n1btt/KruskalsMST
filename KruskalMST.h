/**
* @file KruskalMST.h
* @authors
*   Francisco Eduardo Fontenele - 15452569
*   Vinicius Botte - 15522900
*
* \brief Implementa o algoritmo de Kruskal para calcular a MST.
*
* \details Usa Union-Find (Disjoint Set) para detectar ciclos e escolhe arestas por ordem de peso.
*          Aplica-se à rede de computadores para construir um backbone econômico conectando todos os nós.
*
* \pre O grafo deve ser não-direcionado e ponderado; deve haver conectividade suficiente para MST.
* \post Retorna o conjunto de arestas da MST e seu custo total.
*/

#ifndef KRUSKAL_MST_H
#define KRUSKAL_MST_H

#include <vector>
#include <string>
#include <unordered_map>
#include "Graph.h"
#include "Edge.h"

using namespace std;

class KruskalMST {
private:
    unordered_map<string, string> parent;
    unordered_map<string, int> rank;

    /**
     * \brief Inicializa estruturas de Union-Find para as chaves fornecidas.
     * \pre Vetor de chaves deve conter todos os vértices do grafo.
     * \post Cada vértice é seu próprio representante e rank inicia em zero.
     */
    void makeSet(const vector<string>& vertices);

    /**
     * \brief Encontra o representante do conjunto usando path compression.
     * \pre A chave deve existir nas estruturas de Union-Find.
     * \post Retorna o líder do conjunto e reduz profundidade da árvore de representantes.
     */
    string findSet(const string& v);

    /**
     * \brief Une dois conjuntos distintos usando union by rank.
     * \pre Ambos representantes devem ser válidos e possivelmente distintos.
     * \post Conjuntos são unidos e ranks atualizados conforme necessário.
     */
    void unionSets(const string& a, const string& b);

public:
    /**
     * \brief Calcula a MST do grafo via Kruskal.
     * \pre O grafo deve estar carregado; se desconexo, retorna MST por componente.
     * \post Retorna arestas selecionadas e custo total por referência de saída.
     */
    void compute(const Graph& g, vector<Edge>& outMst, int& outTotalCost);
};

#endif
