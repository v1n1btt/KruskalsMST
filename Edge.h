/**
* @file Edge.h
* @authors
*   Francisco Eduardo Fontenele - 15452569
*   Vinicius Botte - 15522900
*
* \brief Declaração da classe Edge que representa uma aresta ponderada não-direcionada.
*
* \details A aresta conecta dois vértices com um custo inteiro, modelando um link físico de rede.
*
* \pre As chaves devem ser válidas no grafo que consumirá esta aresta.
* \post Instâncias podem ser comparadas por peso e usadas em algoritmos de MST.
*/

#ifndef EDGE_H
#define EDGE_H

#include <string>
using namespace std;

class Edge {
private:
    string a;
    string b;
    int cost;

public:
    /**
     * \brief Constrói uma aresta entre dois vértices com custo.
     * \pre As chaves podem ser iguais, porém não é recomendado; custo deve ser inteiro válido.
     * \post A aresta é inicializada e pronta para uso.
     */
    Edge(const string& va, const string& vb, int c) : a(va), b(vb), cost(c) {}

    /**
     * \brief Obtém a primeira extremidade.
     * \pre A aresta deve estar inicializada.
     * \post Retorna a chave do primeiro vértice.
     */
    const string& u() const { return a; }

    /**
     * \brief Obtém a segunda extremidade.
     * \pre A aresta deve estar inicializada.
     * \post Retorna a chave do segundo vértice.
     */
    const string& v() const { return b; }

    /**
     * \brief Obtém o custo da aresta.
     * \pre A aresta deve estar inicializada.
     * \post Retorna o custo inteiro associado.
     */
    int weight() const { return cost; }

    /**
     * \brief Operador de ordenação por custo (menor primeiro).
     * \pre Ambas arestas devem estar inicializadas.
     * \post Permite ordenação por peso para uso em Kruskal.
     */
    bool operator<(const Edge& other) const { return cost < other.cost; }
};

#endif
