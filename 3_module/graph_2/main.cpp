// made by <github.com/silvman> 2018
#include <iostream>
#include <vector>
#include <queue>
#include <cassert>

/*
    Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то вершинами.
    Найдите количество различных кратчайших путей между заданными вершинами. Требуемая сложность O(V+E).
    Формат ввода.
    v: кол-во вершин (макс. 50000),
    n: кол-во ребер (макс. 200000),
    n пар реберных вершин,
    пара вершин u, w для запроса.
    Формат вывода.
    Количество кратчайших путей от u к w.
*/

struct Graph {
    virtual ~Graph() = default;

    virtual void AddEdge(int from, int to) = 0;
    virtual unsigned long VerticesCount() const = 0;

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0;
};

class ListGraph : public Graph {
public:
    explicit ListGraph(unsigned long count);
    explicit ListGraph(const Graph& graph);
    ~ListGraph() override = default;

    void AddEdge(int from, int to) override;
    unsigned long VerticesCount() const override;
    void GetNextVertices(int vertex, std::vector<int> &vertices) const override;
    void GetPrevVertices(int vertex, std::vector<int> &vertices) const override;

private:
    std::vector<std::vector<int>> adj_list;
};


int NumMinWays(const Graph& graph, int from, int to) {
    std::vector<int> min_way;
    std::vector<int> ways_count;
    min_way.resize(graph.VerticesCount(), -1); // -1 - означает, что вершина еще не была обработана
    ways_count.resize(graph.VerticesCount(), 0);

    std::queue<int> qu;
    qu.push(from);
    min_way[from] = 0;
    ways_count[from] = 1;

    while (!qu.empty()) {
        int current = qu.front();
        qu.pop();

        std::vector<int> adjacentVertexes;
        graph.GetNextVertices(current, adjacentVertexes);
        for (auto &vertex : adjacentVertexes) {
            if (min_way[vertex] == -1 || min_way[vertex] > min_way[current] + 1) { // если вершина еще не была обработана или существует более короткий путь
                min_way[vertex] = min_way[current] + 1;
                ways_count[vertex] = ways_count[current];
                qu.push(vertex);
            } else if (min_way[vertex] == min_way[current] + 1) {
                ways_count[vertex] += ways_count[current];  // если существуют разные пути одинаковой длины до одной вершины, суммируем
            }
        }
    }

    return ways_count[to];
}



int main() {
    int v = 0;
    std::cin >> v; // вершины

    int n = 0;
    std::cin >> n; // ребра

    ListGraph graph(v);
    for (int i = 0; i < n; ++i) {
        int from = 0, to = 0;
        std::cin >> from >> to;
        graph.AddEdge(from, to); // добавляем оба направления, т.к. граф неориентированный
        graph.AddEdge(to, from);
    }

    int from = 0, to = 0;
    std::cin >> from >> to; // откуда и куда

    std::cout << NumMinWays(graph, from, to) << std::endl;
    return 0;
}



void ListGraph::AddEdge(int from, int to) {
    assert( from >= 0 && from < adj_list.size() );
    assert( to >= 0 && to < adj_list.size() );

    adj_list[from].push_back(to);
}

unsigned long ListGraph::VerticesCount() const {
    return adj_list.size();

}

void ListGraph::GetNextVertices(int vertex, std::vector<int> &vertices) const {
    assert( vertex >= 0 && vertex < VerticesCount() );
    vertices = adj_list[vertex];
}

void ListGraph::GetPrevVertices(int vertex, std::vector<int> &vertices) const {
    assert( vertex >= 0 && vertex < VerticesCount() );

    for (int from = 0; from < adj_list.size(); ++from)
        for (int i = 0; i < adj_list[from].size(); ++i)
            if (adj_list[from][i] == vertex)
                vertices.push_back(from);
}

ListGraph::ListGraph(unsigned long count) {
    assert( count > 0 );
    adj_list.resize(count);
}

ListGraph::ListGraph(const Graph &graph) {
    adj_list.resize(graph.VerticesCount());
    for (int from = 0; from < adj_list.size(); ++from)
        graph.GetNextVertices(from, adj_list[from]);
}
