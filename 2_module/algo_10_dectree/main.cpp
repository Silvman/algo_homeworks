// made by <github.com/silvman> 2018
#include <iostream>
#include <cassert>
#include <vector>
#include <queue>

/*
 * Дано число N < 10^6 и последовательность пар целых чисел из [- 2^31, 2^31] длиной N.
 * Построить декартово дерево из N узлов, характеризующихся парами чисел (Xi, Yi).
 * Каждая пара чисел (Xi, Yi) определяет ключ Xi и приоритет Yi в декартовом дереве.
 * Добавление узла в декартово дерево выполняйте второй версией алгоритма, рассказанного на лекции:
 * При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим приоритетом.
 * Затем разбейте найденное поддерево по ключу x так, чтобы в первом поддереве все ключи меньше x,
 * а во втором больше или равны x. Получившиеся два дерева сделайте дочерними для нового узла (x, y).
 * Новый узел вставьте на место узла P. Построить также наивное дерево поиска по ключам Xi.
 * Равные ключи добавляйте в правое поддерево.
 *
 * Вычислить количество узлов в самом широком слое декартового дерева и количество узлов в самом широком слое
 * наивного дерева поиска. Вывести их разницу. Разница может быть отрицательна.
 */


// декартово дерево
template <typename T, typename U>
struct treap_node {
    T       key;
    U  priority;
    treap_node<T, U>* left;
    treap_node<T, U>* right;

    treap_node(const T& key, const U& priority);
    ~treap_node();
};

template<typename T, typename U>
treap_node<T, U>::treap_node(const T &key, const U &priority)
        : key(key), priority(priority), left(nullptr), right(nullptr) { }

template<typename T, typename U>
treap_node<T, U>::~treap_node() {
    delete left;
    delete right;
}

template <typename T, typename U>
class treap {
    treap_node<T, U>* root;

    void split(treap_node<T, U>* current_node, const T& key,
               treap_node<T, U>* & left, treap_node<T, U>* & right);

    treap_node<T, U> * merge(treap_node<T, U> *left, treap_node<T, U> *right);

public:
    treap();
    ~treap();

    void add (const T& key, const U& priority);
    void remove (const T& key);
    int get_max_width();

};

template<typename T, typename U>
treap<T, U>::~treap() {
    delete root;
}

// left - место куда подвешивается левое дерево
template<typename T, typename U>
void treap<T, U>::split(treap_node<T, U> *current_node, const T &key,
                        treap_node<T, U> *&left, treap_node<T, U> *&right) {
    if (!current_node) {
        left = right = nullptr;
    } else if (current_node->key <= key) {
        split(current_node->right, key, current_node->right, right);
        left = current_node;
    } else {
        split(current_node->left, key, left, current_node->left);
        right = current_node;
    }
}

template<typename T, typename U>
treap_node<T, U> * treap<T, U>::merge(treap_node<T, U> *left, treap_node<T, U> *right) {
    if (!left)
        return right;

    if (!right)
        return left;

    if (left->priority > right->priority) {
        left->right = merge(left->right, right);
        return left;
    } else {
        right->left = merge(left, right->left);
        return right;
    }
}

template<typename T, typename U>
void treap<T, U>::add(const T &key, const U &priority) {
    if (root) {
        treap_node<T, U>* current = root;

        if (current->priority < priority) {
            auto *new_node = new treap_node<T, U>(key, priority);
            split(current, key, new_node->left, new_node->right);
            root = new_node;
            return;
        }

        while (true) {
            if(key < current->key) {
                if (current->left) {
                    if (current->left->priority < priority) {
                        auto *new_node = new treap_node<T, U>(key, priority);
                        split(current->left, key, new_node->left, new_node->right);
                        current->left = new_node;
                        return;
                    }

                    current = current->left;
                } else {
                    current->left = new treap_node<T, U>(key, priority);
                    return;
                }
            } else {
                if (current->right) {
                    if (current->right->priority < priority) {
                        auto *new_node = new treap_node<T, U>(key, priority);
                        split(current->right, key, new_node->left, new_node->right);
                        current->right = new_node;
                        return;
                    }

                    current = current->right;
                } else {
                    current->right = new treap_node<T, U>(key, priority);
                    return;
                }
            }
        }
    } else {
        root = new treap_node<T, U>(key, priority);
    }
}

template<typename T, typename U>
void treap<T, U>::remove(const T &key) {
    if (!root)
        return;

    treap_node<T, U>* current = root;
    while (true) {
        if(current->left && current->left->key == key) {
            current->left = merge(current->left->right, current->left->left);
            return;
        }

        if(current->right && current->right->key == key) {
            current->right = merge(current->right->right, current->right->left);
            return;
        }

        if(key < current->key) {
            if (current->left) {
                current = current->left;
            } else {
                break;
            }
        } else {
            if (current->right) {
                current = current->right;
            } else {
                break;
            }
        }
    }
}

template<typename T, typename U>
treap<T, U>::treap() : root(nullptr) { }

template<typename T, typename U>
int treap<T, U>::get_max_width() {
    std::vector<treap_node<T, U>*> vec_1;
    std::vector<treap_node<T, U>*> vec_2;
    int max_count = 0;

    vec_1.push_back(root);
    while( !vec_1.empty() || !vec_2.empty() ) {
        int counter = 0;
        for (treap_node<T, U>* item : vec_1) {
            counter++;
            if (item->left)
                vec_2.push_back(item->left);

            if (item->right)
                vec_2.push_back(item->right);
        }
        if (counter > max_count)
            max_count = counter;

        vec_1.clear();

        counter = 0;
        for (treap_node<T, U>* item : vec_2) {
            counter++;
            if (item->left)
                vec_1.push_back(item->left);

            if (item->right)
                vec_1.push_back(item->right);
        }
        if (counter > max_count)
            max_count = counter;

        vec_2.clear();
    }

    return max_count;
}



// наивная реализация
template <typename T>
struct binary_tree_node {
    binary_tree_node<T>* left;
    binary_tree_node<T>* right;
    //    binary_tree_node<T>* parent;

    T data;

    explicit binary_tree_node(const T& data)
            : right(nullptr), left(nullptr), data(data) { }
};

template <typename T>
class binary_tree {
private:
    binary_tree_node<T>* root;

public:
    binary_tree() : root(nullptr) { }
    ~binary_tree();

    void insert (const T& key);
    int get_max_width();

};

template<typename T>
binary_tree<T>::~binary_tree() {
    if(root) {
        std::queue<binary_tree_node<T> *> nodes;

        nodes.push(root);
        while (!nodes.empty()) {
            binary_tree_node<T> *node = nodes.front(); // TODO front или back?
            if (node->left)
                nodes.push(node->left);

            if (node->right)
                nodes.push(node->right);

            delete node;
            nodes.pop();
        }
    }
}

template<typename T>
void binary_tree<T>::insert(const T &key) {
    if (root) {
        binary_tree_node<T>* current = root;
        while (true) {
            if(current->data > key) {
                if (current->left) {
                    current = current->left;
                } else {
                    current->left = new binary_tree_node<T>(key);
                    break;
                }
            } else {
                if (current->right) {
                    current = current->right;
                } else {
                    current->right = new binary_tree_node<T>(key);
                    break;
                }
            }
        }
    } else {
        root = new binary_tree_node<T>(key);
    }

}

template<typename T>
int binary_tree<T>::get_max_width() {
    std::vector<binary_tree_node<T>*> vec_1;
    std::vector<binary_tree_node<T>*> vec_2;
    int max_count = 0;

    vec_1.push_back(root);
    while( !vec_1.empty() || !vec_2.empty() ) {
        int counter = 0;
        for (binary_tree_node<T>* item : vec_1) {
            counter++;
            if (item->left)
                vec_2.push_back(item->left);

            if (item->right)
                vec_2.push_back(item->right);
        }
        if (counter > max_count)
            max_count = counter;

        vec_1.clear();

        counter = 0;
        for (binary_tree_node<T>* item : vec_2) {
            counter++;
            if (item->left)
                vec_1.push_back(item->left);

            if (item->right)
                vec_1.push_back(item->right);
        }
        if (counter > max_count)
            max_count = counter;

        vec_2.clear();
    }

    return max_count;
}



int main() {
    treap<int, int> decart_tree;
    binary_tree<int> bin_tree;

    int count = 0;
    std::cin >> count;
    assert(count > 0);

    for (int i = 0; i < count; ++i) {
        int num = 0;
        int priority = 0;
        std::cin >> num >> priority;
        decart_tree.add(num, priority);
        bin_tree.insert(num);
    }

    std::cout << decart_tree.get_max_width() - bin_tree.get_max_width();

    return 0;
}
