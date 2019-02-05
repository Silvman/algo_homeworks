// made by <github.com/silvman> 2018
#include <iostream>
#include <queue>
#include <stack>
#include <cassert>

/*
 * Дано число N < 10^6 и последовательность целых чисел из [-2^31..2^31] длиной N.
 * Требуется построить бинарное дерево, заданное наивным порядком вставки.
 * Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
 * то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
 * Выведите элементы в порядке pre-order (сверху вниз).
 *
 * Рекурсия запрещена.
 */


template <typename T>
struct binary_tree_node {
    binary_tree_node<T>* left;
    binary_tree_node<T>* right;

    T data;

    explicit binary_tree_node(const T& data)
            : right(nullptr), left(nullptr), data(data) { }
};

template <typename T>
class binary_tree {
private:
    binary_tree_node<T>* root;

    void delete_node(binary_tree_node<T>* & node);
    binary_tree_node<T>* find_parent (const T& key) const;

public:
    binary_tree() : root(nullptr) { }
    ~binary_tree();

    binary_tree_node<T>* find (const T& key) const;
    binary_tree_node<T>* find_min (const T& key) const;
    binary_tree_node<T>* find_max (const T& key) const;
    void insert (const T& key);
    bool remove (const T& key);
    std::vector<T> pre_order();
};

template<typename T>
binary_tree<T>::~binary_tree() {
    // обход по слоям
    if (root) {
        std::queue<binary_tree_node<T> *> nodes;

        nodes.push(root);
        while (!nodes.empty()) {
            binary_tree_node<T> *node = nodes.front();
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
binary_tree_node<T> *binary_tree<T>::find_min(const T &key) const {
    if (!root)
        return nullptr;

    binary_tree_node<T>* current = root;
    while(current->left != nullptr)
        current = current->left;

    return current;
}

template<typename T>
binary_tree_node<T> *binary_tree<T>::find_max(const T &key) const {
    if (!root)
        return nullptr;

    binary_tree_node<T>* current = root;
    while(current->right != nullptr)
        current = current->right;

    return current;
}

template<typename T>
binary_tree_node<T> *binary_tree<T>::find_parent(const T &key) const {
    if (!root)
        return nullptr;

    binary_tree_node<T>* current = root;
    while (true) {
        if(     (current->left   && current->left->data  == key)
             || (current->right  && current->right->data == key) )
            return current;

        if(key < current->data) {
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

    return nullptr;

}

template<typename T>
binary_tree_node<T> * binary_tree<T>::find(const T &key) const {
    if (!root)
        return nullptr;

    binary_tree_node<T>* current = root;
    while (true) {
        if(current->data == key) return current;
        if(key < current->data) {
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

    return nullptr;
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
bool binary_tree<T>::remove(const T &key) {
    // ищем родителя, для того чтобы затем передать по ссылке указатель на его потомка.
    auto parent = find_parent(key);
    if (!parent)
        return false;

    if (parent->left && parent->left->data == key)
        delete_node(parent->left);
    else if (parent->right && parent->right->data == key)
        delete_node(parent->right);

    return true;
}

template<typename T>
void binary_tree<T>::delete_node(binary_tree_node<T>* & node) {
    if (!node->left) { // нет левого потомка
        auto right = node->right;
        delete node;
        node = right;
    } else if (!node->right) { // нет правого
        auto left = node->left;
        delete node;
        node = left;
    } else {    // есть оба
        auto min_parent = node; // минимальный
        auto min = node->right; // его родитель, чтобы можно было перенести возможных бОльших (правых) потомков минимального
        while (min->left) {
            min_parent = min;
            min = min->left;
        }

        node->data = min->data;
        min_parent->left = min->right; // переносим потомка минимального на его родителя

        delete min;
    }
}

template<typename T>
std::vector<T> binary_tree<T>::pre_order() {
    assert(root);
    std::vector<T> keys;

    std::stack<binary_tree_node<T> *> nodes;
    nodes.push(root);
    while (!nodes.empty()) {
        binary_tree_node<T>* node = nodes.top();
        nodes.pop();

        if (node->right)
            nodes.push(node->right);


        if (node->left)
            nodes.push(node->left);

        keys.push_back(node->data);
    }

    return keys;
}

int main() {
    binary_tree<int> tree;

    int count = 0;
    std::cin >> count;
    assert(count > 0);

    for (int i = 0; i < count; ++i) {
        int num = 0;
        std::cin >> num;
        tree.insert(num);
    }

    std::vector<int>pre_order_keys = tree.pre_order();
    for (auto &&key : pre_order_keys) {
        std::cout << key << " ";
    }

    return 0;
}
