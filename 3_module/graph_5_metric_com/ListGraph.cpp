//
// Created by silvman on 28.05.18.
//

#include <cassert>
#include "ListGraph.h"

void ListGraph::AddEdge(int from, int to) {
    assert( from >= 0 && from < adjacencyList.size() );
    assert( to >= 0 && to < adjacencyList.size() );

    adjacencyList[from].emplace_back(to);
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
                vertices.emplace_back(from);
}

ListGraph::ListGraph(unsigned long count) {
    assert( count > 0 );
    adjacencyList.resize(count + 1);
}

ListGraph::ListGraph(const Graph &graph) {
    adjacencyList.resize(graph.VerticesCount());
    for (int from = 0; from < adjacencyList.size(); ++from)
        graph.GetNextVertices(from, adjacencyList[from]);
}
