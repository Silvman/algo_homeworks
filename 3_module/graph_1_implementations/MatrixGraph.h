//
// Created by silvman on 27.05.18.
//

#ifndef ALGRO_GRAPH_1_MATRIXGRAPH_H
#define ALGRO_GRAPH_1_MATRIXGRAPH_H

#include "Graph.h"

class MatrixGraph: public Graph {
private:
    std::vector<std::vector<int>> matrix;

public:
    MatrixGraph(unsigned long count);
    MatrixGraph(const Graph& graph);
    ~MatrixGraph() override = default;

    void AddEdge(int from, int to) override;
    unsigned long VerticesCount() const override;
    void GetNextVertices(int vertex, std::vector<int> &vertices) const override;
    void GetPrevVertices(int vertex, std::vector<int> &vertices) const override;
};


#endif //ALGRO_GRAPH_1_MATRIXGRAPH_H
