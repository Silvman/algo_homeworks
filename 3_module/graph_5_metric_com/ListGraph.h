//
// Created by silvman on 28.05.18.
//

#ifndef ALGO_GRAPH_5_LISTGRAPH_H
#define ALGO_GRAPH_5_LISTGRAPH_H

#include "Graph.h"

class ListGraph : public Graph {
public:
    explicit ListGraph(unsigned long count);
    explicit ListGraph(const Graph& graph);
    ~ListGraph() override = default;

    void AddEdge(int from, int to) override;
    unsigned long VerticesCount() const override;
    void GetNextVertices(int vertex, std::vector<int> &vertices) const override;
    void GetPrevVertices(int vertex, std::vector<int> &vertices) const override;

private:
    std::vector<std::vector<int>> adjacencyList;
};

#endif //ALGO_GRAPH_5_LISTGRAPH_H
