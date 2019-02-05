//
// Created by silvman on 28.05.18.
//

#ifndef ALGO_GRAPH_5_GRAPH_H
#define ALGO_GRAPH_5_GRAPH_H

#include <vector>

struct Graph {
    virtual ~Graph() = default;

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;
    virtual unsigned long VerticesCount() const = 0;

    virtual void GetNextVertices(int vertex, std::vector<int> &vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector<int> &vertices) const = 0;
};

#endif //ALGO_GRAPH_5_GRAPH_H
