/**
* @file main.cpp
 * @authors
 *   Francisco Eduardo Fontenele - 15452569
 *   Vinicius Botte - 15522900
 *
 * AED II - Trabalho 2
 */

#include "Graph.h"
#include <iostream>
#include <filesystem>

using namespace std;

/**
 * @brief Limpa estado de erro e descarta até o fim da linha no stdin.
 */
static void clearInputLine() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static string readLine(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

/**
 * @brief Lê um inteiro no intervalo [minV..maxV] com re-prompt até validar.
 * @param prompt Mensagem exibida.
 * @param minV Valor mínimo aceito.
 * @param maxV Valor máximo aceito.
 * @return Inteiro validado.
 */
static int readIntInRange(const string& prompt, int minV, int maxV) {
    while (true) {
        cout << prompt;
        int v;
        if (cin >> v) {
            if (v >= minV && v <= maxV) {
                clearInputLine();
                return v;
            }
            cout << "Valor invalido. Informe um numero entre " << minV << " e " << maxV << "." << endl;
        } else {
            cout << "Entrada invalida. Tente novamente." << endl;
            clearInputLine();
        }
    }
}

/**
 * @brief Lê qualquer inteiro com re-prompt até validar.
 * @param prompt Mensagem exibida.
 * @return Inteiro lido.
 */
static int readAnyInt(const string& prompt) {
    while (true) {
        cout << prompt;
        int v;
        if (cin >> v) {
            clearInputLine();
            return v;
        } else {
            cout << "Entrada invalida. Tente novamente." << endl;
            clearInputLine();
        }
    }
}

/**
 * @brief Lê resposta 's' ou 'n' (case-insensitive) com re-prompt.
 * @param prompt Mensagem exibida.
 * @return 's' ou 'n'.
 */
static char readYesNo(const string& prompt) {
    while (true) {
        cout << prompt;
        char c;
        if (cin >> c) {
            clearInputLine();
            if (c == 's' || c == 'S') return 's';
            if (c == 'n' || c == 'N') return 'n';
            cout << "Responda com 's' ou 'n'." << endl;
        } else {
            cout << "Entrada invalida. Tente novamente." << endl;
            clearInputLine();
        }
    }
}

int main() {
    string binFile = "graph.bin";

    filesystem::path cwd = filesystem::current_path();
    filesystem::path binPath = cwd / binFile;

    if (!filesystem::exists(binPath)) {
        ofstream bin(binPath, ios::binary | ios::trunc);
        if (!bin.is_open()) return -1;
        int n = 0;
        bin.write(reinterpret_cast<const char*>(&n), sizeof(int));
        bin.flush();
        bin.close();
    }

    Graph graph(binPath.string());
    
    while(true) {
        graph.displayGraph();

        cout << "Selecione uma opcao:" << endl;
        cout << "1. Buscar vértice" << endl;
        cout << "2. Inserir vértice" << endl;
        cout << "3. Inserir aresta" << endl;
        cout << "4. Remover vértice" << endl;
        cout << "5. Remover aresta" << endl;
        cout << "6. Sair" << endl;

        int opt = readIntInRange("Escolha (1-6): ", 1, 6);

        switch (opt)
        {
            case 1:

                break;
            
            case 2:
                {
                    string key = readLine("Chave para inserir: ");
                    graph.insertVertex(key);
                }
                break;

            case 3:
                {
                    string keyA = readLine("Primeiro vertice: ");
                    string keyB = readLine("Segundo vertice: ");
                    int cost = readAnyInt("Custo da aresta: ");
                    graph.insertEdge(keyA, keyB, cost);
                }
                break;

            case 6:
                return 0;
        }
    }
}