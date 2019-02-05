// made by <github.com/silvman> 2018
#include <iostream>
#include <vector>
#include <queue>
#include "ListGraph.h"
#include "ArcGraph.h"
#include "SetGraph.h"
#include "MatrixGraph.h"

void BFS(const Graph& graph, int vertex, void (*visit)(int)) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::queue<int> qu;
    qu.push(vertex);
    visited[vertex] = true;

    while (!qu.empty()) {
        int current = qu.front();
        qu.pop();
        visit(current);
        std::vector<int> adjacentVertexes;
        graph.GetNextVertices(current, adjacentVertexes);
        for (auto &&adjVertex : adjacentVertexes) {
            if ( !visited[adjVertex] ) {
                qu.push(adjVertex);
                visited[adjVertex] = true;
            }
        }
    }
}

int main() {
    SetGraph graph(7);
    graph.AddEdge(0, 1);
    graph.AddEdge(0, 5);
    graph.AddEdge(1, 2);
    graph.AddEdge(1, 3);
    graph.AddEdge(1, 5);
    graph.AddEdge(1, 6);
    graph.AddEdge(3, 2);
    graph.AddEdge(3, 6);
    graph.AddEdge(4, 3);
    graph.AddEdge(5, 4);
    graph.AddEdge(5, 6);
    graph.AddEdge(6, 4);

    MatrixGraph mGr = SetGraph(ArcGraph(ListGraph(graph)));

    BFS(mGr, 0, [](int vertex) { std::cout << vertex << " "; });
    return 0;
}
