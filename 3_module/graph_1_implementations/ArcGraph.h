//
// Created by silvman on 27.05.18.
//

#ifndef ALGRO_GRAPH_1_ARCGRAPH_H
#define ALGRO_GRAPH_1_ARCGRAPH_H

#include "Graph.h"

class ArcGraph : public Graph {
public:
    ArcGraph(unsigned long count);
    ArcGraph(const Graph& graph);
    ~ArcGraph() override = default;

    void AddEdge(int from, int to) override;

    unsigned long VerticesCount() const override;

    void GetNextVertices(int vertex, std::vector<int> &vertices) const override;

    void GetPrevVertices(int vertex, std::vector<int> &vertices) const override;


private:
    std::vector<std::pair<int, int>> arcs;
    unsigned long count;

};


#endif //ALGRO_GRAPH_1_ARCGRAPH_H
