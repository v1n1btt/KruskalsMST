# KruskalsMST

Este projeto implementa, em C++17, uma ferramenta de linha de comando para construir e manipular um grafo não direcionado e ponderado (modelo de rede de computadores), persistido em arquivo binário, e para calcular sua Árvore Geradora Mínima (MST) usando o algoritmo de Kruskal com Union-Find (Disjoint Set). O foco é acadêmico em ciência da computação, cobrindo estrutura de dados, complexidade, persistência e parsing de topologia textual.

## Visão Geral da Arquitetura

- Componentes principais:
  - `Graph`: grafo não-direcionado e ponderado via lista de adjacência, com persistência binária.
  - `Edge`: aresta imutável com dois vértices e custo; ordenável por peso.
  - `KruskalMST`: algoritmo de Kruskal usando Union-Find com path compression e union by rank.
  - `NetworkTopologyParser`: parser de arquivo `.txt` com linhas `VERTEX` e `EDGE`.
  - `main`: CLI com menu iterativo para operações CRUD e execução do Kruskal.
- Persistência:
  - O grafo é vinculado a um arquivo binário (`graph.bin`) no diretório de execução.
  - A leitura ocorre no construtor de `Graph`; a escrita ocorre no destrutor e em `clearAndPersist`.
- Fluxo de execução típico:
  1. Inicialização do binário (cabeçalho com `n = 0` vértices).
  2. Carregar topologia (texto) ou inserir manualmente vértices e arestas.
  3. Consultar/alterar elementos.
  4. Executar Kruskal para obter MST e custo total.
  5. Sair, persistindo estado.

## Como cada módulo funciona

### Edge.h (Aresta)
- Representa uma aresta não-direcionada com custo inteiro.
- Campos:
  - `a`, `b`: rótulos de vértices (strings).
  - `cost`: peso/custo.
- Métodos:
  - `u()`, `v()`, `weight()`: acesso imutável aos campos.
  - `operator<`: permite ordenação por custo (usado por Kruskal).
- Papel:
  - Unidade básica para computação de MST. Comparação por custo garante ordenação estável por peso.

### Graph.h / Graph.cpp (Grafo e Persistência)
- Estrutura:
  - `adjList`: `map<string, list<pair<string,int>>>` armazenando vizinhos e custos.
  - `filename`: caminho do binário persistente.
- Persistência binária:
  - Formato:
    - `int n`: número de vértices.
    - Para cada vértice: `string key` (serializada como `int len` + bytes), `int degree`, e pares (`string vizinho`, `int custo`).
  - `readGraph` carrega todo o estado; `writeGraph` grava todo o estado.
  - `clearAndPersist` zera o grafo e regrava binário com `n = 0`.
- Operações principais:
  - `insertVertex(key)`: cria entrada vazia se não existir.
  - `insertEdge(a,b,cost)`: adiciona aresta em ambos sentidos se ambos vértices existirem e aresta não existe.
  - `deleteUndirectedEdge(a,b)`: remove ambos lados da aresta.
  - `deleteVertex(key)`: remove vértice e todas as arestas incidentes.
  - `hasVertex(key)`, `hasEdge(a,b,&cost)`: busca.
  - `getEdgesUnique()`: retorna arestas únicas (critério `u < v` para evitar duplicidade).
  - `getAllVertices()`: retorna todas as chaves de vértices (sem ordem garantida).
  - `displayGraph()`: imprime lista de adjacência para observabilidade.
- Importação de topologia:
  - `importFromTxt(path)`: via `NetworkTopologyParser::parse`, insere vértices e arestas com custos.

### NetworkTopologyParser.h / .cpp (Parser de Topologia)
- Formato textual esperado:
  - Linha `VERTEX <chave>`
  - Linha `EDGE <chaveA> <chaveB> <custo>`
- Exemplo:
  - `VERTEX R1`
  - `VERTEX R2`
  - `EDGE R1 R2 10`
- Operação:
  - Lê o arquivo linha a linha; popula `outVertices` e `outEdges`.
  - Tolerante a linhas vazias; simples e determinístico.

### KruskalMST.h / .cpp (Algoritmo de Kruskal)
- Estruturas:
  - `parent: unordered_map<string,string>` para Union-Find.
  - `rank: unordered_map<string,int>` para union by rank.
- Passos:
  1. `compute(g, outMst, outTotalCost)`: coleta arestas únicas e vértices do grafo.
  2. Ordena arestas por peso (`std::sort`, O(E log E)).
  3. Inicializa conjuntos disjuntos (`makeSet`).
  4. Itera arestas em ordem crescente; se `findSet(u) != findSet(v)`, inclui aresta e une conjuntos.
  5. Retorna `outMst` e `outTotalCost`.
- Propriedades:
  - Path compression em `findSet` e union by rank em `unionSets` garantem quase O(α(n)) amortizado.
  - Se o grafo for desconexo, retorna uma floresta geradora mínima (MST por componente).

### main.cpp (CLI)
- Responsável pelo loop de interação:
  - Imprime estado do grafo (`displayGraph`).
  - Menu de opções 0–10:
    - 0: limpar grafo e binário.
    - 1: carregar `data/simple_topology.txt`.
    - 2: carregar `data/complex_topology.txt`.
    - 3: buscar vértice.
    - 4: buscar aresta (com custo se existir).
    - 5: inserir vértice.
    - 6: inserir aresta (com custo).
    - 7: remover vértice.
    - 8: remover aresta.
    - 9: executar Kruskal (imprime MST e custo total).
    - 10: sair.
- Funções utilitárias:
  - `ensureBinaryInit`: cria `graph.bin` com cabeçalho `n=0` se não existir.
  - `readLine`, `readInt`, `readIntInRange`: leitura robusta do terminal.
  - `dataPath`: resolve pasta `data` ao lado do executável.

## Complexidade e Considerações

- Armazenamento:
  - Lista de adjacência com mapa (log n para acesso por chave).
- Kruskal:
  - Ordenação: O(E log E).
  - Union-Find: quase constante amortizado por operação.
  - Total: O(E log E + E α(V)) ~ O(E log E).
- Persistência:
  - Serializa completamente o grafo a cada `writeGraph` (no destrutor e em `clearAndPersist`).
- Integridade:
  - Arestas duplicadas evitadas por `getEdgesUnique`.
  - Inserção de aresta falha silenciosamente se vértices não existirem (design simples de CLI).

## Build e Execução

Pré-requisitos: CMake >= 3.16, compilador C++17.

Build:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Execução:
```bash
./KruskalsMST   # no Linux/macOS
KruskalsMST.exe # no Windows
```

Durante a execução, o programa cria/usa `graph.bin` no diretório atual e copia `data/` ao lado do executável (via CMake pós-build).

## Formato de Arquivo de Topologia (.txt)

- Cada linha é um comando:
  - `VERTEX X`
  - `EDGE X Y W`
- Exemplo mínimo:
  ```
  VERTEX A
  VERTEX B
  EDGE A B 5
  ```

Coloque esses arquivos em `data/` (já copiado para ao lado do binário pelo pós-build).

## Casos de Teste Guiados (Entradas e Saídas Exatas)

Os testes abaixo usam exatamente os arquivos presentes em `data/` e mostram literalmente o que digitar e a saída esperada. Execute no diretório do binário com a pasta `data/` ao lado.

### Caminho 1: Topologia simples (data/simple_topology.txt)
Conteúdo:
```
VERTEX R1
VERTEX R2
VERTEX R3
EDGE R1 R2 10
EDGE R2 R3 5
EDGE R1 R3 8
```

Passos:
1) Carregar o arquivo:
```
Escolha (0-10): 1
```
2) Executar Kruskal:
```
Escolha (0-10): 9
```
3) Sair:
```
Escolha (0-10): 10
```

Saída esperada no passo 2:
```
MST (Kruskal)
-------------
R2 - R3 (5)
R1 - R3 (8)
Custo total: 13
```

Critério de correção:
- Duas arestas conectam R1, R2, R3 sem ciclo.
- Soma dos pesos é exatamente 13.

### Caminho 2: Topologia complexa (data/complex_topology.txt)
Conteúdo:
```
VERTEX R1
VERTEX R2
VERTEX R3
VERTEX R4
VERTEX R5
VERTEX R6
VERTEX R7
VERTEX R8
EDGE R1 R2 4
EDGE R2 R3 2
EDGE R3 R4 7
EDGE R4 R5 3
EDGE R5 R6 6
EDGE R6 R7 1
EDGE R7 R8 9
EDGE R1 R3 5
EDGE R2 R4 11
EDGE R3 R5 10
EDGE R4 R6 12
EDGE R5 R7 8
EDGE R6 R8 13
EDGE R1 R8 15
EDGE R2 R7 14
```

Passos:
1) Carregar o arquivo:
```
Escolha (0-10): 2
```
2) Executar Kruskal:
```
Escolha (0-10): 9
```
3) Sair:
```
Escolha (0-10): 10
```

Saída esperada no passo 2:
```
MST (Kruskal)
-------------
R6 - R7 (1)
R2 - R3 (2)
R4 - R5 (3)
R1 - R2 (4)
R5 - R6 (6)
R3 - R4 (7)
R7 - R8 (9)
Custo total: 32
```

Critério de correção:
- Sete arestas conectam todos os 8 vértices sem ciclo.
- Soma dos pesos é exatamente 32.

### Caminho 3: Inserção manual, busca e MST
Passos:
1) Limpar:
```
Escolha (0-10): 0
```
2) Inserir vértice A:
```
Escolha (0-10): 5
Chave do novo vertice: A
```
3) Inserir vértice B:
```
Escolha (0-10): 5
Chave do novo vertice: B
```
4) Inserir aresta A-B custo 10:
```
Escolha (0-10): 6
Primeiro vertice: A
Segundo vertice: B
Custo da aresta: 10
```
Saída esperada após inserção:
```
OK.
```
5) Buscar aresta A-B:
```
Escolha (0-10): 4
Primeiro vertice: A
Segundo vertice: B
```
Saída esperada da busca:
```
Custo 10.
```
6) Executar Kruskal:
```
Escolha (0-10): 9
```
Saída esperada:
```
MST (Kruskal)
-------------
A - B (10)
Custo total: 10
```
7) Sair:
```
Escolha (0-10): 10
```

Observações rápidas
- Mensagens de erro são curtas e objetivas: “Invalido.”, “Chave vazia.”, “Auto-aresta bloqueada.”, “Custo negativo.”, “Vertice inexistente.”, “Ja existe.”, “Nao existe.”.
- Em grafos desconexos, a saída é uma floresta mínima por componente.

## Dicas de Uso e Erros Comuns

- Inserir aresta sem inserir vértices antes: a operação não terá efeito (verifique com `displayGraph`).
- Formato de topologia incorreto: linhas com tokens insuficientes serão ignoradas; corrija o arquivo.
- Grafo desconexo: MST impressa é uma floresta; o número de arestas não será `n-1` global.
- Persistência: ao sair (opção `10`) o estado atual é gravado no `graph.bin`. Use `0` para limpar completamente.

## Estrutura de Diretórios

- `CMakeLists.txt`: configuração de build, flags e cópia da pasta `data` pós-build.
- `data/`: arquivos de topologia de exemplo (`simple_topology.txt`, `complex_topology.txt`).
- `graph.bin`: persistência do grafo no diretório de execução (gerado em runtime).
- Código-fonte: `*.h` e `*.cpp` para cada módulo descrito.

## Extensões Sugeridas (Acadêmicas)

- Validação de entrada no parser (detecção de erros e mensagens).
- Suporte a múltiplas métricas de custo (latência, banda, custo composto).
- Visualização do grafo e MST em formato DOT/Graphviz.
- Testes automatizados com GoogleTest para cenários de conectividade e pesos.

## Visualização da MST e do Grafo (Python)

Após rodar o programa em C++ e executar a opção 9 (Kruskal), são gerados três arquivos texto no diretório atual (ou na pasta de build):
- graph_vertices.txt
- graph_edges.txt
- mst_edges.txt

Esses arquivos são consumidos pelo script visualize.py para desenhar o grafo, destacando as arestas da MST.

### Pré-requisitos

Instale Python 3 (>=3.8) e pip. Depois instale as dependências:
```bash
pip install networkx matplotlib
```

Opcional (ambiente isolado):
```bash
python -m venv .venv
# Ativar:
# Windows PowerShell: .venv\Scripts\Activate.ps1
# Linux/macOS: source .venv/bin/activate
pip install networkx matplotlib
```

### Fluxo Completo

1. Build e execute o binário.
2. No menu, carregue uma topologia (opções 1 ou 2) ou construa manualmente.
3. Escolha a opção 9 (Executar Kruskal). Isso:
   - Imprime a MST.
   - Exporta os arquivos texto (vertices, arestas, mst).
4. Rode o script Python para visualizar.

Se você pular a opção 9, mst_edges.txt pode não existir (apenas grafo será exibido sem destaque de MST).

### Uso do Script

Formato:
```bash
python visualize.py [imagem_saida.png|.svg] [base_dir]
```
- Primeiro argumento (opcional): nome do arquivo de imagem a salvar (PNG/SVG). Se omitido, abre janela interativa.
- Segundo argumento (opcional): diretório base onde procurar os arquivos. Se omitido, usa o diretório atual.
- O script tenta também base_dir/cmake-build-debug automaticamente (útil em CLion).

Exemplos (Windows):
```bash
cd C:\Users\eduar\CLionProjects\KruskalsMST\cmake-build-debug
python ..\visualize.py          # apenas exibe
python ..\visualize.py mst.png  # salva imagem mst.png
python ..\visualize.py mst.svg  # salva SVG
python ..\visualize.py mst.png C:\Users\eduar\CLionProjects\KruskalsMST
```

Exemplos (Linux/macOS):
```bash
cd /caminho/para/KruskalsMST/build
python ../visualize.py
python ../visualize.py resultado.png
```

### Cores e Destaques

- Arestas da MST: azul (#1f77b4), espessura 3.0.
- Arestas comuns: cinza (#888888), espessura 1.5.
- Nós: cinza claro (#d3d3d3).
- Pesos: exibidos como rótulos nas arestas (se inteiros válidos).

### Salvando Imagem

Para garantir exportação:
```bash
python visualize.py mst.png
```
Saída esperada no terminal:
```
Imagem salva em: mst.png
```

### Solução de Problemas

- Erro "Não encontrado: graph_edges.txt":
  - Execute novamente o binário e escolha opção 9 (gera os arquivos).
  - Verifique se está no diretório onde o .exe rodou (ex: cmake-build-debug).
- Arquivos vazios:
  - Certifique-se de ter inserido vértices/arestas antes de rodar Kruskal.
- MST não destacada:
  - mst_edges.txt pode estar ausente → rode opção 9 de novo.

### Workflow Rápido (Topologia Simples)

```bash
# Rodar binário
KruskalsMST.exe
# Menu: escolha 1 (carregar simples), depois 9 (Kruskal), depois 10 (sair)
python visualize.py mst_simple.png C:\Users\eduar\CLionProjects\KruskalsMST\cmake-build-debug
```

### Limpeza / Regeração

- Use opção 0 para limpar grafo e depois reconstrua antes de nova visualização.
- Repetir opção 9 sempre que modificar o grafo para atualizar mst_edges.txt.

### Integração Futura

- Pode-se adicionar requirements.txt e incluir novas libs (ex: pygraphviz) para layouts alternativos.
- Exportar em DOT e consumir por outras ferramentas.
