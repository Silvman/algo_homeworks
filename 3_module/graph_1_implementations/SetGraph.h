//
// Created by silvman on 27.05.18.
//

#ifndef ALGRO_GRAPH_1_SETGRAPH_H
#define ALGRO_GRAPH_1_SETGRAPH_H

#include <unordered_set>
#include "Graph.h"

class SetGraph : public Graph {
public:
    SetGraph(unsigned long count);
    SetGraph(const Graph& graph);
    ~SetGraph() override = default;

    void AddEdge(int from, int to) override;
    unsigned long VerticesCount() const override;
    void GetNextVertices(int vertex, std::vector<int> &vertices) const override;
    void GetPrevVertices(int vertex, std::vector<int> &vertices) const override;

private:
    std::vector<std::unordered_set<int>> adjacencySet;
};


#endif //ALGRO_GRAPH_1_SETGRAPH_H
