// made by <github.com/silvman> 2018
#include <iostream>
#include <vector>
#include <queue>

/*
    Вариант 1. Для построения минимального остовного дерева используйте алгоритм Крускала.
    В контесте протестируйте работу алгоритма построения минимального остовного дерева.
 */

class DSU {
public:
    DSU(int count);
    int FindParent(int v);
    void Union(int v1, int v2);

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

DSU::DSU(int count) :
        parent(count + 1), // делаем на 1 больше, так как в контесте номера вершин идут с 1
        rank(count + 1, 1) {
    for (int i = 0; i <= count; i++)
        parent[i] = i;
}

int DSU::FindParent(int v) {
    if (v == parent[v])
        return v;
    parent[v] = FindParent(parent[v]);
    return parent[v];
}

void DSU::Union(int v1, int v2) {
    int p1 = FindParent(v1);
    int p2 = FindParent(v2);
    if (p1 == p2)
        return;

    if (rank[p1] < rank[p2])
        parent[p1] = p2;
    else {
        parent[p2] = p1;
        if (rank[p1] == rank[p2])
            rank[p1]++;
    }
}

struct edge {
    edge(int from, int to, int weight) : from(from), to(to), weight(weight) {}
    int from{0}, to{0}, weight{0};
};

struct edge_bigger {
    bool operator () (const edge& a, const edge& b) const {
        return a.weight > b.weight;
    }
};

int main() {
    int n = 0, m = 0;
    int min_size = 0;
    std::cin >> n >> m; // вершины, рёбра
    DSU dsu(n);
    std::priority_queue<edge, std::vector<edge>, edge_bigger> edges; // очередь с приоритетом (куча), работает как пирамидальная сортировка по весу ребра (наименьшие сверху)

    for (int i = 0; i < m; ++i) {
        int from = 0, to = 0, weight = 0;
        std::cin >> from >> to >> weight;
        edges.emplace(from, to, weight);
    }

    while (!edges.empty()) {
        edge current = edges.top();
        if (dsu.FindParent(current.from) != dsu.FindParent(current.to)) {
            dsu.Union(current.from, current.to);
            min_size += current.weight;
        }

        edges.pop();
    }

    std::cout << min_size << std::endl;

    return 0;
}
