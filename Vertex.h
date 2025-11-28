/**
* @file Vertex.h
* @authors
*   Francisco Eduardo Fontenele - 15452569
*   Vinicius Botte - 15522900
*
* \brief Declaração da classe Vertex que representa um vértice (nó) de rede.
*
* \details Um vértice representa um dispositivo de rede (roteador/switch) identificado por uma chave.
*
* \pre A chave deve ser não vazia para uso semântico no grafo.
* \post Instâncias podem ser usadas em contêineres associativos e comparadas por igualdade.
*/

#ifndef VERTEX_H
#define VERTEX_H

#include <string>
using namespace std;

class Vertex {
private:
    string key;

public:
    /**
     * \brief Constrói um vértice com a chave fornecida.
     * \pre A chave pode ser vazia, porém recomenda-se chave única e não vazia.
     * \post O vértice é inicializado e pronto para uso.
     */
    explicit Vertex(const string& k) : key(k) {}

    /**
     * \brief Obtém a chave do vértice.
     * \pre O vértice deve estar inicializado.
     * \post Retorna a chave associada a este vértice.
     */
    const string& getKey() const { return key; }

    /**
     * \brief Operador de igualdade baseado na chave.
     * \pre Ambos vértices devem estar inicializados.
     * \post Retorna verdadeiro se as chaves forem idênticas.
     */
    bool operator==(const Vertex& other) const { return key == other.key; }

    /**
     * \brief Operador de ordenação para uso em contêineres ordenados.
     * \pre Ambos vértices devem estar inicializados.
     * \post Retorna verdadeiro se a chave atual for lexicograficamente menor.
     */
    bool operator<(const Vertex& other) const { return key < other.key; }
};

#endif
