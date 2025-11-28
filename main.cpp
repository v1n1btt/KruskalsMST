/**
* @file main.cpp
* @authors
*   Francisco Eduardo Fontenele - 15452569
*   Vinicius Botte - 15522900
*
* \brief Interface de linha de comando para manipular a rede e executar Kruskal.
*
* \details Oferece menu para criar/importar topologia, inserir/remover vértices/arestas,
*          buscar elementos e imprimir MST com custo total.
*
* \pre Executar em diretório com permissões de leitura/escrita; Windows suportado.
* \post Operações persistem no arquivo binário ao encerrar o programa.
*/

#include "Graph.h"
#include "KruskalMST.h"
#include "SimpleExporter.h"
#include <iostream>
#include <filesystem>
#include <limits>
#include <fstream>
#include <string>
using namespace std;

/**
 * \brief Limpa o buffer de entrada padrão.
 * \pre Deve haver uma operação de leitura anterior possivelmente com falha.
 * \post Fluxo é limpo e pronto para próxima leitura de linha.
 */
static void clearInputLine() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/**
 * \brief Lê uma linha da entrada com prompt.
 * \pre Saída padrão deve estar acessível para exibir o prompt.
 * \post Retorna a string lida do usuário.
 */
static string readLine(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

/**
 * \brief Lê um inteiro da entrada com validação.
 * \pre Entrada padrão deve estar acessível.
 * \post Retorna um inteiro válido do usuário.
 */
static int readInt(const string& prompt) {
    while (true) {
        cout << prompt;
        int v;
        if (cin >> v) { clearInputLine(); return v; }
        cout << "\nEntrada invalida.\n";
        clearInputLine();
    }
}

/**
 * \brief Lê um inteiro em intervalo definido.
 * \pre Valores minimo e maximo devem definir um intervalo valido.
 * \post Retorna inteiro dentro do intervalo especificado.
 */
static int readIntInRange(const string& prompt, int minV, int maxV) {
    while (true) {
        int v = readInt(prompt);
        if (v >= minV && v <= maxV) return v;
        cout << "\nValor invalido.\n";
    }
}

/**
 * \brief Retorna o caminho da pasta de dados.
 * \pre A pasta deve estar disponível ao lado do executável.
 * \post Caminho absoluto para `data` é retornado.
 */
static filesystem::path dataPath() {
    return filesystem::current_path() / "data";
}

/**
 * \brief Garante existência do arquivo binário inicial.
 * \pre Caminho alvo deve ser válido e gravável.
 * \post Arquivo binário é criado com cabeçalho inicial se não existir.
 */
static void ensureBinaryInit(const filesystem::path& binPath) {
    if (!filesystem::exists(binPath)) {
        ofstream bin(binPath, ios::binary | ios::trunc);
        if (!bin.is_open()) {
            cout << "Falha ao criar arquivo binario.\n";
            std::exit(-1);
        }
        int magic = 0x47524150, version = 1, n = 0;
        bin.write(reinterpret_cast<const char*>(&magic), sizeof(int));
        bin.write(reinterpret_cast<const char*>(&version), sizeof(int));
        bin.write(reinterpret_cast<const char*>(&n), sizeof(int));
    }
}

static string trim(const string& s) {
    size_t i = 0, j = s.size();
    while (i < j && isspace(static_cast<unsigned char>(s[i]))) ++i;
    while (j > i && isspace(static_cast<unsigned char>(s[j - 1]))) --j;
    return s.substr(i, j - i);
}

/**
 * \brief Imprime o estado atual do grafo.
 * \pre O grafo deve estar carregado.
 * \post Exibe o número de vértices e a lista de adjacência.
 */
static void printGraphState(const Graph& graph) {
    graph.displayGraph();
}

/**
 * \brief Imprime o menu contínuo de operações.
 * \pre Saída padrão deve estar acessível.
 * \post Exibe todas as opções em formato simples com espaçamento.
 */
static void printMenu() {
    cout << "Menu de operacoes\n";
    cout << "-----------------\n";
    cout << "0. Limpar grafo e binario\n";
    cout << "1. Carregar topologia simples (data/simple_topology.txt)\n";
    cout << "2. Carregar topologia complexa (data/complex_topology.txt)\n";
    cout << "3. Buscar vertice\n";
    cout << "4. Buscar aresta\n";
    cout << "5. Inserir vertice\n";
    cout << "6. Inserir aresta\n";
    cout << "7. Remover vertice\n";
    cout << "8. Remover aresta\n";
    cout << "9. Executar Kruskal (MST)\n";
    cout << "10. Sair\n\n";
}

/**
 * \brief Ponto de entrada da aplicação CLI.
 * \pre Ambiente com C\+\+17, acesso a arquivos e diretórios de trabalho válido.
 * \post Executa loop de menu até opção de saída e persiste estado ao finalizar.
 */
int main() {
    string binFile = "graph.bin";
    filesystem::path binPath = filesystem::current_path() / binFile;

    ensureBinaryInit(binPath);

    Graph graph(binPath.string());
    KruskalMST kruskal;

    while (true) {
        printGraphState(graph);
        printMenu();
        int opt = readIntInRange("Escolha (0-10): ", 0, 10);

        if (opt == 0) {
            graph.clearAndPersist();
            cout << "\nOK.\n\n";
        } else if (opt == 1) {
            auto path = dataPath() / "simple_topology.txt";
            graph.importFromTxt(path.string());
        } else if (opt == 2) {
            auto path = dataPath() / "complex_topology.txt";
            graph.importFromTxt(path.string());
        } else if (opt == 3) {
            string key = trim(readLine("\nChave do vertice: "));
            if (key.empty()) { cout << "Invalido.\n\n"; continue; }
            bool ok = graph.hasVertex(key);
            cout << (ok ? "Existe.\n\n" : "Nao existe.\n\n");
        } else if (opt == 4) {
            string a = trim(readLine("\nPrimeiro vertice: "));
            string b = trim(readLine("Segundo vertice: "));
            if (a.empty() || b.empty()) { cout << "Invalido.\n\n"; continue; }
            int cost = 0;
            bool ok = graph.hasEdge(a, b, &cost);
            if (ok) cout << "Custo " << cost << ".\n\n";
            else cout << "Nao encontrada.\n\n";
        } else if (opt == 5) {
            string key = trim(readLine("\nChave do novo vertice: "));
            if (key.empty()) { cout << "Chave vazia.\n\n"; continue; }
            if (graph.hasVertex(key)) { cout << "Ja existe.\n\n"; continue; }
            graph.insertVertex(key);
            cout << "OK.\n\n";
        } else if (opt == 6) {
            string a = trim(readLine("\nPrimeiro vertice: "));
            string b = trim(readLine("Segundo vertice: "));
            if (a.empty() || b.empty()) { cout << "Invalido.\n\n"; continue; }
            if (a == b) { cout << "Auto-aresta bloqueada.\n\n"; continue; }
            int w = readInt("Custo da aresta: ");
            if (w < 0) { cout << "Custo negativo.\n\n"; continue; }
            if (!graph.hasVertex(a) || !graph.hasVertex(b)) { cout << "Vertice inexistente.\n\n"; continue; }
            int c;
            if (graph.hasEdge(a, b, &c)) { cout << "Ja existe.\n\n"; continue; }
            graph.insertEdge(a, b, w);
            cout << "OK.\n\n";
        } else if (opt == 7) {
            string key = trim(readLine("\nVertice a remover: "));
            if (key.empty()) { cout << "Invalido.\n\n"; continue; }
            if (!graph.hasVertex(key)) { cout << "Nao existe.\n\n"; continue; }
            graph.deleteVertex(key);
            cout << "OK.\n\n";
        } else if (opt == 8) {
            string a = trim(readLine("\nPrimeiro vertice: "));
            string b = trim(readLine("Segundo vertice: "));
            if (a.empty() || b.empty()) { cout << "Invalido.\n\n"; continue; }
            if (!graph.hasVertex(a) || !graph.hasVertex(b)) { cout << "Vertice inexistente.\n\n"; continue; }
            if (!graph.hasEdge(a, b)) { cout << "Nao existe.\n\n"; continue; }
            graph.deleteUndirectedEdge(a, b);
            cout << "OK.\n\n";
        } else if (opt == 9) {
            vector<Edge> mst;
            int total = 0;
            kruskal.compute(graph, mst, total);
            cout << "\nMST (Kruskal)\n";
            cout << "-------------\n";
            for (const auto& e : mst) {
                cout << e.u() << " - " << e.v() << " (" << e.weight() << ")\n";
            }
            cout << "Custo total: " << total << "\n\n";

            const string vPath = "graph_vertices.txt";
            const string ePath = "graph_edges.txt";
            const string mPath = "mst_edges.txt";
            SimpleExporter::exportVertices(graph, vPath);
            SimpleExporter::exportEdges(graph, ePath);
            SimpleExporter::exportMst(mst, mPath);
            cout << "Exportados: " << vPath << ", " << ePath << ", " << mPath << "\n\n";
        } else if (opt == 10) {
            return 0;
        }
    }
}
