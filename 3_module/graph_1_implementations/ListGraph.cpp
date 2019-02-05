//
// Created by silvman on 27.05.18.
//

#include <cassert>
#include "ListGraph.h"

void ListGraph::AddEdge(int from, int to) {
    assert( from >= 0 && from < VerticesCount() );
    assert( to >= 0 && to < VerticesCount() );

    adjacencyList[from].push_back(to);
}

unsigned long ListGraph::VerticesCount() const {
    return adjacencyList.size();
}

void ListGraph::GetNextVertices(int vertex, std::vector<int> &vertices) const {
    assert( vertex >= 0 && vertex < VerticesCount() );
    vertices = adjacencyList[vertex];
}

void ListGraph::GetPrevVertices(int vertex, std::vector<int> &vertices) const {
    assert( vertex >= 0 && vertex < VerticesCount() );

    for (int from = 0; from < adjacencyList.size(); ++from)
        for (int i = 0; i < adjacencyList[from].size(); ++i)
            if (adjacencyList[from][i] == vertex)
                vertices.push_back(from);
}

ListGraph::ListGraph(unsigned long count) {
    assert( count > 0 );
    adjacencyList.resize(count);
}

ListGraph::ListGraph(const Graph &graph) {
    adjacencyList.resize(graph.VerticesCount());
    for (int from = 0; from < adjacencyList.size(); ++from)
        graph.GetNextVertices(from, adjacencyList[from]);
}
