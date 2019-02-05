//
// Created by silvman on 27.05.18.
//

#include <cassert>
#include "SetGraph.h"


void SetGraph::AddEdge(int from, int to) {
    assert( from >= 0 && from < VerticesCount() );
    assert( to >= 0 && to < VerticesCount() );

    adjacencySet[from].insert(to);
}

unsigned long SetGraph::VerticesCount() const {
    return adjacencySet.size();
}

void SetGraph::GetNextVertices(int vertex, std::vector<int> &vertices) const {
    assert( vertex >= 0 && vertex < VerticesCount() );

    for (auto &&item : adjacencySet[vertex])
        vertices.push_back(item);
}

void SetGraph::GetPrevVertices(int vertex, std::vector<int> &vertices) const {
    assert( vertex >= 0 && vertex < VerticesCount() );

    for (int i = 0; i < VerticesCount(); ++i)
        if (adjacencySet[i].find(vertex) != adjacencySet[i].end())
            vertices.push_back(i);
}

SetGraph::SetGraph(unsigned long count) {
    assert( count > 0 );
    adjacencySet.resize(count);
}

SetGraph::SetGraph(const Graph &graph) {
    adjacencySet.resize(graph.VerticesCount());
    for (int i = 0; i < adjacencySet.size(); ++i) {
        std::vector<int> nextVertexes;
        graph.GetNextVertices(i, nextVertexes);
        for (auto &&vertex : nextVertexes) {
            adjacencySet[i].insert(vertex);
        }
    }
}