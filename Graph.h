/**
* @file Graph.h
* @authors
*   Francisco Eduardo Fontenele - 15452569
*   Vinicius Botte - 15522900
*
* \brief Classe que representa um grafo não-direcionado e ponderado usando lista de adjacência.
*
* \details Focado em redes de computadores: vértices são dispositivos e arestas são links com custo.
*          Oferece operações de criação, inserção, remoção, busca, impressão, importação de .txt
*          e persistência em arquivo binário.
*
* \pre O arquivo binário será criado se não existir; chaves devem ser adequadas ao contexto.
* \post Instâncias mantêm estado consistente e operações atualizam a persistência ao finalizar.
*/

#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <list>
#include <string>
#include <vector>
#include <fstream>
#include "Edge.h"
using namespace std;

class Graph {
private:
    string filename;
    map<string, list<pair<string, int>>> adjList;
    static constexpr int MAGIC = 0x47524150;
    static constexpr int VERSION = 1;

    static string readString(ifstream& in);
    map<string, list<pair<string, int>>> readGraph();
    static void writeString(ofstream& out, const string& s);
    void writeGraph() const;

public:
    /**
     * \brief Constrói o grafo vinculado a um arquivo binário.
     * \pre O caminho deve ser válido; cria arquivo com cabeçalho se necessário.
     * \post O grafo é carregado e pronto para operações.
     */
    explicit Graph(const string& filename_);

    /**
     * \brief Destrói o grafo persistindo alterações.
     * \pre O objeto deve estar em estado consistente.
     * \post O arquivo binário é atualizado com o estado final.
     */
    ~Graph();

    /**
     * \brief Imprime o grafo completo.
     * \pre O grafo deve estar carregado.
     * \post A saída padrão contém a lista de adjacência.
     */
    void displayGraph() const;

    /**
     * \brief Busca um vértice.
     * \pre A chave não deve ser vazia.
     * \post Retorna verdadeiro se existir.
     */
    bool hasVertex(const string& key) const;

    /**
     * \brief Busca uma aresta.
     * \pre As chaves devem estar definidas.
     * \post Retorna verdadeiro se houver aresta; opcionalmente retorna custo.
     */
    bool hasEdge(const string& a, const string& b, int* outCost = nullptr) const;

    /**
     * \brief Insere um vértice.
     * \pre A chave não deve colidir.
     * \post O vértice passa a existir.
     */
    void insertVertex(const string& key);

    /**
     * \brief Insere uma aresta não-direcionada com custo.
     * \pre Ambos vértices devem existir.
     * \post A aresta é adicionada nos dois sentidos.
     */
    void insertEdge(const string& a, const string& b, int cost);

    /**
     * \brief Remove uma aresta não-direcionada.
     * \pre Ambos vértices devem existir.
     * \post A aresta é removida de ambos os lados.
     */
    void deleteUndirectedEdge(const string& a, const string& b);

    /**
     * \brief Remove um vértice e incidentes.
     * \pre O vértice deve existir.
     * \post O vértice e ligações são removidos.
     */
    void deleteVertex(const string& key);

    /**
     * \brief Recupera arestas únicas.
     * \pre O grafo deve estar carregado.
     * \post Retorna vetor de arestas sem duplicidade.
     */
    vector<Edge> getEdgesUnique() const;

    /**
     * \brief Importa topologia de arquivo texto.
     * \pre O arquivo deve seguir o formato suportado.
     * \post Vértices e arestas são adicionados.
     */
    void importFromTxt(const string& path);

    /**
     * \brief Limpa o grafo e regrava o binário vazio.
     * \pre O arquivo binário deve ser acessível para escrita.
     * \post O grafo fica vazio e o arquivo binário contém cabeçalho com zero vértices.
     */
    void clearAndPersist();

    /**
     * \brief Retorna todas as chaves de vértices do grafo.
     * \pre O grafo deve estar carregado.
     * \post Vetor com as chaves existentes, sem ordem garantida.
     */
    vector<string> getAllVertices() const;
};

#endif
