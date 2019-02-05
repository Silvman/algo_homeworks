//
// Created by silvman on 27.05.18.
//

#include <cassert>
#include "MatrixGraph.h"

void MatrixGraph::AddEdge(int from, int to) {
    assert( from >= 0 && from < VerticesCount() );
    assert( to >= 0 && to < VerticesCount() );

    matrix[from][to] = 1;
}

unsigned long MatrixGraph::VerticesCount() const {
    return matrix.size();
}

void MatrixGraph::GetNextVertices(int vertex, std::vector<int> &vertices) const {
    assert( vertex >= 0 && vertex < VerticesCount() );

    for (int i = 0; i < VerticesCount(); ++i)
        if (matrix[vertex][i])
            vertices.push_back(i);
}

void MatrixGraph::GetPrevVertices(int vertex, std::vector<int> &vertices) const {
    assert( vertex >= 0 && vertex < VerticesCount() );

    for (int i = 0; i < VerticesCount(); ++i)
        if (matrix[i][vertex])
            vertices.push_back(i);
}

MatrixGraph::MatrixGraph(unsigned long count) {
    assert( count > 0 );

    matrix.resize(count);
    for (auto &&item : matrix)
        item.resize(count);
}

MatrixGraph::MatrixGraph(const Graph &graph) {
    matrix.resize(graph.VerticesCount());
    for (auto &&item : matrix)
        item.resize(graph.VerticesCount());

    for (int i = 0; i < graph.VerticesCount(); ++i) {
        std::vector<int> nextVertices;
        graph.GetNextVertices(i, nextVertices);
        for (auto &&vertex : nextVertices)
            matrix[i][vertex] = 1;
    }
}
