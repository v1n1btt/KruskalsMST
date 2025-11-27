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
        
    }
}