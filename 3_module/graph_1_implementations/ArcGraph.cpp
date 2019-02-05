//
// Created by silvman on 27.05.18.
//

#include <cassert>
#include "ArcGraph.h"


void ArcGraph::AddEdge(int from, int to) {
    assert( from >= 0 && from < VerticesCount() );
    assert( to >= 0 && to < VerticesCount() );

    arcs.emplace_back(from, to);
}

unsigned long ArcGraph::VerticesCount() const {
    return count;
}

void ArcGraph::GetNextVertices(int vertex, std::vector<int> &vertices) const {
    assert( vertex >= 0 && vertex < VerticesCount() );

    for (auto &&arc : arcs)
        if (arc.first == vertex)
            vertices.push_back(arc.second);
}

void ArcGraph::GetPrevVertices(int vertex, std::vector<int> &vertices) const {
    assert( vertex >= 0 && vertex < VerticesCount() );

    for (auto &&arc : arcs)
        if (arc.second == vertex)
            vertices.push_back(arc.first);
}

ArcGraph::ArcGraph(unsigned long count) : count(count) {
    assert( count > 0 );
}

ArcGraph::ArcGraph(const Graph &graph) : count(graph.VerticesCount()) {
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        std::vector<int> nextVertexes;
        graph.GetNextVertices(i, nextVertexes);
        for (auto &&vertex : nextVertexes)
            arcs.emplace_back(i, vertex);
    }
}
