/**
* @file NetworkTopologyParser.cpp
* @authors
*   Francisco Eduardo Fontenele - 15452569
*   Vinicius Botte - 15522900
*
* \brief Implementação do parser de topologia de rede.
*
* \details Converte o arquivo texto em listas de vértices e arestas com validação básica.
*
* \pre O arquivo deve estar acessível para leitura.
* \post Vetores de saída são preenchidos; erros são reportados opcionalmente.
*/

#include "NetworkTopologyParser.h"
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

static string trim(const string& s) {
    size_t i = 0, j = s.size();
    while (i < j && isspace(static_cast<unsigned char>(s[i]))) ++i;
    while (j > i && isspace(static_cast<unsigned char>(s[j - 1]))) --j;
    return s.substr(i, j - i);
}

bool NetworkTopologyParser::parse(const string& path,
                                  vector<string>& outVertices,
                                  vector<Edge>& outEdges,
                                  vector<string>* outErrors) {
    outVertices.clear();
    outEdges.clear();
    if (outErrors) outErrors->clear();

    ifstream in(path);
    if (!in) {
        if (outErrors) outErrors->push_back("Arquivo nao encontrado: " + path);
        return false;
    }

    string line;
    int lineNo = 0;
    int validCount = 0;

    while (getline(in, line)) {
        ++lineNo;
        line = trim(line);
        if (line.empty()) continue;

        istringstream iss(line);
        string type;
        iss >> type;

        if (type == "VERTEX") {
            string key;
            iss >> key;
            if (key.empty()) {
                if (outErrors) outErrors->push_back("Linha " + to_string(lineNo) + ": VERTEX sem chave.");
                continue;
            }
            outVertices.push_back(key);
            ++validCount;
        } else if (type == "EDGE") {
            string a, b;
            int w;
            if (!(iss >> a >> b >> w)) {
                if (outErrors) outErrors->push_back("Linha " + to_string(lineNo) + ": EDGE incompleta (esperado: EDGE A B W).");
                continue;
            }
            if (a.empty() || b.empty()) {
                if (outErrors) outErrors->push_back("Linha " + to_string(lineNo) + ": chaves vazias em EDGE.");
                continue;
            }
            if (w < 0) {
                if (outErrors) outErrors->push_back("Linha " + to_string(lineNo) + ": custo negativo em EDGE.");
                continue;
            }
            outEdges.emplace_back(a, b, w);
            ++validCount;
        } else {
            if (outErrors) outErrors->push_back("Linha " + to_string(lineNo) + ": comando desconhecido '" + type + "'.");
        }
    }

    return validCount > 0;
}
