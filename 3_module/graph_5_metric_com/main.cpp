// made by <github.com/silvman> 2018
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
#include "DSU.h"
#include "ListGraph.h"

/*
    Найдите приближенное решение метрической неориентированной задачи коммивояжера в полном графе (на плоскости) с помощью минимального остовного дерева.
    Оцените качество приближения на случайном наборе точек, нормально распределенном на плоскости с дисперсией 1. Нормально распределенный набор точек получайте с помощью преобразования Бокса-Мюллера.
    При фиксированном N, количестве вершин графа, несколько раз запустите оценку качества приближения. Вычислите среднее значение и среднеквадратичное отклонение качества приближения для данного N.
    Запустите данный эксперимент для всех N в некотором диапазоне, например, [2, 10].
    Автоматизируйте запуск экспериментов.
 */

#define RANDOM_SCALE 1000
#define NUM_SOLUTIONS 10


struct edge {
    edge(int from, int to, float weight) : from(from), to(to), weight(weight) {}

    int from{0}, to{0};
    float weight{0};
};

struct edge_bigger {
    bool operator()(const edge &a, const edge &b) const {
        return a.weight > b.weight;
    }
};

std::vector<int> optimise_dst(const Graph &graph, int start_vertex) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::vector<int> optimised; // тут вершины в порядке обхода
    std::stack<int> qu;
    qu.push(start_vertex);

    while (!qu.empty()) {
        int current = qu.top();
        optimised.push_back(current);
        visited[current] = true;
        qu.pop();
        std::vector<int> adjacentVertexes;
        graph.GetNextVertices(current, adjacentVertexes);
        for (auto &&adjVertex : adjacentVertexes)
            if (!visited[adjVertex])
                qu.push(adjVertex);
    }

    optimised.push_back(start_vertex); // вернуться в начало
    return optimised;
}

struct point { // Преобразование Бокса — Мюллера
    point() : x(cosf(2 * M_PI *random_num())*sqrtf(-2 * log2f(random_num()))),
              y(sinf(2 * M_PI *random_num())*sqrtf(-2 * log2f(random_num()))) { }

    float x;
    float y;

private:
    float random_num() {
        return (float)(rand() % RANDOM_SCALE + 1Ко) / RANDOM_SCALE;
    }
};

struct field {
    std::vector<point> vertexes;

    field(int count) {
        for (int i = 0; i < count; ++i)
            vertexes.emplace_back();
    }

    float get_dest(int a, int b) {
        return sqrtf(
                (vertexes[a].x - vertexes[b].x) * (vertexes[a].x - vertexes[b].x) +
                (vertexes[a].y - vertexes[b].y) * (vertexes[a].y - vertexes[b].y)
        );
    }
};

std::vector<std::pair<float, float>> do_solutions() {
    // пара: first - среднее отношение полученного пути к MST, silvman - среднеквадратическое отклонение
    std::vector<std::pair<float, float>> results;

    for (int l = 2; l <= 10; ++l) {
        std::vector<float> quality;

        for (int m = 0; m < NUM_SOLUTIONS; ++m) {
            field field(l);
            DSU dsu(l);
            ListGraph graph(l);
            std::priority_queue<edge, std::vector<edge>, edge_bigger> edges;
            // вместо вектора + сортировка O(n*log(n)) используем очередь с приоритетом, которая куча.
            // O(n*log(n)) silvman добавление n элементов + O(n*log(n)) получение максимума. Как пирамидальная сортировка

            // составляем рёбра полного графа
            for (int j = 0; j < l - 1; ++j) {
                for (int i = j; i < l; ++i) {
                    edges.emplace(i, j, field.get_dest(i, j));
                }
            }

            float mst_length = 0; // вес минимального остовного дерева для качества решения
            while (!edges.empty()) {
                edge current = edges.top();
                if (dsu.FindParent(current.from) != dsu.FindParent(current.to)) { // если не возникает цикла
                    dsu.Union(current.from, current.to); // в одно множество
                    mst_length += current.weight;
                    graph.AddEdge(current.from, current.to); // добавляем два пути - прямой и обратный для оптимизации
                    graph.AddEdge(current.to, current.from);
                }

                edges.pop();
            }

            auto order = optimise_dst(graph, 0);

            float way_length = 0; // получившийся путь
            for (int k = 0; k < order.size() - 1; ++k) {
                way_length += field.get_dest(order[k], order[k+1]);
            }

            quality.push_back(way_length / mst_length);
        }

        float mid_quality = 0;
        for (auto &&value : quality)
            mid_quality += value;
        mid_quality /= quality.size();

        float mid_square_quality = 0;
        for (auto &&value : quality)
            mid_square_quality += (value - mid_quality) * (value - mid_quality);
        mid_square_quality = sqrtf(mid_square_quality / quality.size());

        results.emplace_back(mid_quality, mid_square_quality);
    }


    return results;
}

int main() {
    srand(time(NULL));

    auto results = do_solutions();
    for (int i = 0; i < results.size(); ++i) {
        std::cout << i + 2 << " vertexes: \t" << results[i].first << " \t" << results[i].second << std::endl;
    }

    return 0;
}
