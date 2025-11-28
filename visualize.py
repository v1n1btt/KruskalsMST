# python
# -*- coding: utf-8 -*-
r"""
Visualização de grafo usando NetworkX/Matplotlib sem Graphviz.
Uso:
  python visualize.py [saida.png|.svg] [base_dir]
Se os arquivos não estiverem no base_dir, tenta automaticamente 'base_dir\cmake-build-debug'.
"""

import os
import sys
import networkx as nx
import matplotlib.pyplot as plt

def resolve_paths(base_dir):
    candidates = [base_dir, os.path.join(base_dir, "cmake-build-debug")]
    for d in candidates:
        vp = os.path.join(d, "graph_vertices.txt")
        ep = os.path.join(d, "graph_edges.txt")
        mp = os.path.join(d, "mst_edges.txt")
        if os.path.exists(ep):
            return vp, ep, mp, d
    # report first candidate for clarity
    vp = os.path.join(candidates[0], "graph_vertices.txt")
    ep = os.path.join(candidates[0], "graph_edges.txt")
    mp = os.path.join(candidates[0], "mst_edges.txt")
    return vp, ep, mp, candidates[0]

def load_graph(vertices_path, edges_path):
    G = nx.Graph()
    if os.path.exists(vertices_path):
        with open(vertices_path, "r", encoding="utf-8") as f:
            for line in f:
                k = line.strip()
                if k:
                    G.add_node(k)
    if not os.path.exists(edges_path):
        raise FileNotFoundError(f"Não encontrado: {edges_path}")
    with open(edges_path, "r", encoding="utf-8") as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) >= 3:
                u, v, w = parts[0], parts[1], parts[2]
                try:
                    wv = int(w)
                except ValueError:
                    wv = None
                G.add_edge(u, v, weight=wv)
    return G

def load_mst(mst_path):
    mst_edges = set()
    if os.path.exists(mst_path):
        with open(mst_path, "r", encoding="utf-8") as f:
            for line in f:
                parts = line.strip().split()
                if len(parts) >= 2:
                    u, v = parts[0], parts[1]
                    mst_edges.add(tuple(sorted((u, v))))
    return mst_edges

def main():
    out_path = sys.argv[1] if len(sys.argv) >= 2 and not sys.argv[1].endswith(".py") else None
    base_dir = sys.argv[2] if len(sys.argv) >= 3 else os.getcwd()

    vertices_path, edges_path, mst_path, used_dir = resolve_paths(base_dir)

    for p in (vertices_path, edges_path):
        if not os.path.exists(p):
            print(f"Não encontrado: {p}")
    if not os.path.exists(edges_path):
        print(f"Dica: rode com base_dir onde o `.exe` gerou os arquivos, ex: 'C:\\Users\\eduar\\CLionProjects\\KruskalsMST\\cmake-build-debug'")
        raise FileNotFoundError(f"Não encontrado: {edges_path}")

    G = load_graph(vertices_path, edges_path)
    mst_edges = load_mst(mst_path)

    pos = nx.spring_layout(G, seed=42)
    edge_labels = {(u, v): ("" if d.get("weight") is None else d.get("weight")) for u, v, d in G.edges(data=True)}

    colors, widths = [], []
    for u, v in G.edges():
        in_mst = tuple(sorted((u, v))) in mst_edges
        colors.append("#1f77b4" if in_mst else "#888888")
        widths.append(3.0 if in_mst else 1.5)

    plt.figure(figsize=(8, 6))
    nx.draw(G, pos, with_labels=True, node_color="#d3d3d3", edge_color=colors, width=widths)
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)
    plt.title(f"Base: {used_dir}")
    plt.tight_layout()

    if out_path:
        plt.savefig(out_path, dpi=160)
        print(f"Imagem salva em: {out_path}")
    else:
        plt.show()

if __name__ == "__main__":
    main()
