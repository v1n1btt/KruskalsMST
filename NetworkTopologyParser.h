/**
* @file NetworkTopologyParser.h
* @authors
*   Francisco Eduardo Fontenele - 15452569
*   Vinicius Botte - 15522900
*
* \brief Parser de topologia de rede a partir de arquivo texto.
*
* \details Formato esperado do arquivo:
*   Linha iniciando com "VERTEX" seguida da chave do dispositivo.
*   Linha iniciando com "EDGE" seguida de chaveA chaveB custo.
*   Exemplo:
*     VERTEX R1
*     VERTEX R2
*     EDGE R1 R2 10
*
* \pre O arquivo deve existir e seguir o formato descrito.
* \post Retorna listas de vértices e arestas para construção do grafo.
*/

#ifndef NETWORK_TOPOLOGY_PARSER_H
#define NETWORK_TOPOLOGY_PARSER_H

#include <string>
#include <vector>
#include "Edge.h"
using namespace std;

class NetworkTopologyParser {
public:
    /**
     * \brief Faz o parsing do arquivo de topologia com validações.
     * \pre `path` deve apontar para arquivo legível.
     * \post Preenche `outVertices` e `outEdges`; retorna `true` se houve ao menos uma linha válida.
     */
    static bool parse(const string& path,
                      vector<string>& outVertices,
                      vector<Edge>& outEdges,
                      vector<string>* outErrors = nullptr);
};

#endif
