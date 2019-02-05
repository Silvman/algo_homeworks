//
// Created by silvman on 28.05.18.
//

#ifndef ALGO_GRAPH_5_DSU_H
#define ALGO_GRAPH_5_DSU_H

#include <vector>

class DSU {
public:
    DSU(int count);
    int FindParent(int v);
    void Union(int v1, int v2);

private:
    std::vector<int> parent;
    std::vector<int> rank;
};


#endif //ALGO_GRAPH_5_DSU_H
