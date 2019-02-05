// made by <github.com/silvman> 2018
#include <iostream>
#include <vector>
#include <queue>
#include <cassert>

/*
    Требуется отыскать самый выгодный маршрут между городами.
    Требуемое время работы O((N+M)logN), где N-количество городов, M-известных дорог между ними.
    Оптимизируйте ввод
    Формат входных данных.
    Первая строка содержит число N – количество городов.
    Вторая строка содержит число M - количество дорог.
    Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
    Последняя строка содержит маршрут (откуда и куда нужно доехать).
    Формат выходных данных.
    Вывести длину самого выгодного маршрута.
*/

struct Graph {
    virtual ~Graph() = default;

    virtual void AddEdge(int from, int to, int weight) = 0;
    virtual unsigned long VerticesCount() const = 0;

    virtual void GetNextVertices(int vertex, std::vector<std::pair<int, int>>& vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector<std::pair<int, int>>& vertices) const = 0;
};

class ListGraph : public Graph {
public:
    explicit ListGraph(unsigned long count);
    explicit ListGraph(const Graph& graph);
    ~ListGraph() override = default;

    void AddEdge(int from, int to, int weight) override;
    unsigned long VerticesCount() const override;
    void GetNextVertices(int vertex, std::vector<std::pair<int, int>> &vertices) const override;
    void GetPrevVertices(int vertex, std::vector<std::pair<int, int>> &vertices) const override;

private:
    // пара: first - номер вершины, second - вес ребра.
    std::vector<std::vector<std::pair<int, int>>> adjacencyList;
};

class bigger_vertex {
public:
    // пара: first - номер вершины, second - текущий минимальный путь до неё
    bool operator () (const std::pair<int, int>& vertex_a, const std::pair<int, int>& vertex_b) {
        return vertex_a.second > vertex_b.second;
    }
};

// алгоритм Дейкстры
int FindMinWay(const Graph& graph, int from, int to) {
    assert( from >= 0 && from < graph.VerticesCount() );
    assert( to >= 0 && to < graph.VerticesCount() );

    std::vector<int> min_way;
    std::vector<bool> visited;
    min_way.resize(graph.VerticesCount(), -1); // -1 - недостижимость вершины (вместо бесконечности)
    visited.resize(graph.VerticesCount(), false); // обработанные вершины, которые нельзя менять
    min_way[from] = 0; // начальная вершина

    // очередь с приоритетом по минимальному пути
    // пара: first - номер вершины, second - текущий минимальный путь до неё
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, bigger_vertex> qu;
    qu.push(std::make_pair(from, min_way[from]));

    while (!qu.empty()) {
        auto current = qu.top();
        // пара: first - номер вершины, куда идем, second - вес ребра
        std::vector<std::pair<int, int>> adjacentVertexes;
        graph.GetNextVertices(current.first, adjacentVertexes);
        for (auto &vertex : adjacentVertexes) {
            if (visited[vertex.first])
                continue;

            // если минимальный путь бесконечность, или можем улучшить оценку
            if (min_way[vertex.first] == -1 || min_way[vertex.first] > min_way[current.first] + vertex.second) {
                min_way[vertex.first] = min_way[current.first] + vertex.second;
                qu.push(std::make_pair(vertex.first, min_way[vertex.first]));
            }
        }

        visited[current.first] = true; // вершина посещена и обработана, путь улушить нельзя
        if (current.first == to) // остальные обрабатывать не обязательно
            break;

        qu.pop();
    }

    return min_way[to];
}

int main() {
    int v = 0;
    std::cin >> v;

    int n = 0;
    std::cin >> n;

    ListGraph graph(v);
    for (int i = 0; i < n; ++i) {
        int from = 0, to = 0, weight = 0;
        std::cin >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
        graph.AddEdge(to, from, weight);
    }

    int from = 0, to = 0;
    std::cin >> from >> to;

    std::cout << FindMinWay(graph, from, to) << std::endl;
    return 0;
}

void ListGraph::AddEdge(int from, int to, int weight) {
    assert( from >= 0 && from < adjacencyList.size() );
    assert( to >= 0 && to < adjacencyList.size() );

    adjacencyList[from].emplace_back(to, weight);
}

unsigned long ListGraph::VerticesCount() const {
    return adjacencyList.size();

}

void ListGraph::GetNextVertices(int vertex, std::vector<std::pair<int, int>> &vertices) const {
    assert( vertex >= 0 && vertex < VerticesCount() );
    vertices = adjacencyList[vertex];
}

void ListGraph::GetPrevVertices(int vertex, std::vector<std::pair<int, int>> &vertices) const {
    assert( vertex >= 0 && vertex < VerticesCount() );

    for (int from = 0; from < adjacencyList.size(); ++from)
        for (int i = 0; i < adjacencyList[from].size(); ++i)
            if (adjacencyList[from][i].first == vertex)
                vertices.emplace_back(from, adjacencyList[from][i].second);
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
