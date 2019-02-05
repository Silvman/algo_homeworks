// made by <github.com/silvman> 2018
#include <iostream>
#include <cassert>

/*
 * В одной военной части решили построить в одну шеренгу по росту.
 * Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя,
 * а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
 * Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены
 * по росту – сначала самые высокие, а в конце – самые низкие. За расстановку солдат
 * отвечал прапорщик, который заметил интересную особенность – все солдаты в части
 * разного роста. Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять
 * солдат, а именно для каждого приходящего солдата указывать, перед каким солдатом в строе
 * он должен становится. Требуемая скорость выполнения команды - O(log n).
 */

template <typename T>
struct avl_tree_node {
    int height;
    int count;
    avl_tree_node<T>* left;
    avl_tree_node<T>* right;

    T key;

    explicit avl_tree_node(const T& key): height(1), count(1), key(key), left(nullptr), right(nullptr) {}

    ~avl_tree_node() {
        delete left;
        delete right;
    }
};

template <typename T>
class avl_tree {
    avl_tree_node<T>* root;

    void left_rotate(avl_tree_node<T> *&center);
    void left_big_rotate(avl_tree_node<T> *&center);
    void right_rotate(avl_tree_node<T> *&center);
    void right_big_rotate(avl_tree_node<T> *&center);

    void insert_node(avl_tree_node<T>* & node, const T& key);
    void find_and_remove_node(avl_tree_node<T> *&node, const T &key);

    T get_and_remove_min(avl_tree_node<T> *&node);
    const T& get_min_key(avl_tree_node<T>* & node);

    void remove_node(avl_tree_node<T> *& node);
    void restore_balance(avl_tree_node<T> *&node);
    void restore_height(avl_tree_node<T> *&node);

    int get_height(avl_tree_node<T>* node) const;
    int get_count(avl_tree_node<T>* node) const;

    int get_position_node(avl_tree_node<T> *& node, const T &key);
    const T& get_key_node(avl_tree_node<T> *& node, int position);

public:
    avl_tree() : root (nullptr) {}
    ~avl_tree() {
        delete root;
    }
// silvman 2018
    void insert(const T& key);
    void remove(const T& key);

    int get_position(const T& key);
    const T& get_key(int position);

};

template<typename T>
void avl_tree<T>::insert(const T &key) {
    insert_node(root, key);
}

template<typename T>
void avl_tree<T>::insert_node(avl_tree_node<T>* & node, const T &key) {
    if (!node) {
        node = new avl_tree_node<T>(key);
        return;
    }

    if (node->key < key) {
        insert_node(node->right, key);
    } else {
        insert_node(node->left, key);
    }

    restore_height(node);
    restore_balance(node);
}

template<typename T>
int avl_tree<T>::get_height(avl_tree_node<T> *node) const {
    if (node)
        return node->height;
    else
        return 0;
}

template<typename T>
void avl_tree<T>::remove(const T &key) {
    find_and_remove_node(root, key);
}

template<typename T>
void avl_tree<T>::find_and_remove_node(avl_tree_node<T> *&node, const T &key) {
    if (!node)
        return;

    if (node->key == key) {
        remove_node(node);
        return;
    }

    if (node->key < key) {
        find_and_remove_node(node->right, key);
    } else {
        find_and_remove_node(node->left, key);
    }

    restore_height(node);
    restore_balance(node);
}



template<typename T>
void avl_tree<T>::remove_node(avl_tree_node<T> *&node) {
    if (!node->left) {
        auto right = node->right;
        node->right = nullptr;
        delete node;
        node = right;
    } else if (!node->right) {
        auto left = node->left;
        node->left = nullptr;
        delete node;
        node = left;
    } else {
        // объединенные в одну функцию поиск минимального и удаление
        node->key = get_and_remove_min(node->right);

        restore_height(node);
        restore_balance(node);
    }
}

template<typename T>
void avl_tree<T>::restore_balance(avl_tree_node<T> *&node) {
    if ( node->right && (get_height(node->left) + 2 == get_height(node->right))
         && (get_height(node->right->left) <= get_height(node->right->right)) ) {
        left_rotate(node);
// github.com/silvman
    } else if ( node->left && (get_height(node->right) + 2 == get_height(node->left))
                && (get_height(node->left->right) <= get_height(node->left->left)) ) {
        right_rotate(node);
    } else if ( node->right && (get_height(node->right->right) == get_height(node->left) + 1)
                && (get_height(node->right->left) == get_height(node->left) + 2) ) {
        left_big_rotate(node);
    } else if ( node->left && (get_height(node->left->left) == get_height(node->right) + 1)
                && (get_height(node->left->right) == get_height(node->right) + 2) ) {
        right_big_rotate(node);
    }
}

template<typename T>
void avl_tree<T>::left_rotate(avl_tree_node<T> *&center) {
    auto* a = center;
    center = center->right;
    auto* c = center->left;
    center->left = a;
    center->left->right = c;

    restore_height(center->left);
    restore_height(center);
}

template<typename T>
void avl_tree<T>::left_big_rotate(avl_tree_node<T> *&center) {
    right_rotate(center->right);
    left_rotate(center);
}

template<typename T>
void avl_tree<T>::right_rotate(avl_tree_node<T> *&center) {
    auto* a = center;
    center = center->left;
    auto* c = center->right;
    center->right = a;
    center->right->left = c;

    restore_height(center->right);
    restore_height(center);
}

template<typename T>
void avl_tree<T>::right_big_rotate(avl_tree_node<T> *&center) {
    left_rotate(center->left);
    right_rotate(center);
}

template<typename T>
void avl_tree<T>::restore_height(avl_tree_node<T> *&node) {
    if (node) {
        node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
        node->count = get_count(node->left) + get_count(node->right) + 1;
    }
}

template<typename T>
int avl_tree<T>::get_position(const T &key) {
    return get_position_node(root, key) - 1;
}

template<typename T>
int avl_tree<T>::get_count(avl_tree_node<T> *node) const {
    if (node)
        return node->count;
    else
        return 0;
}

template<typename T>
int avl_tree<T>::get_position_node(avl_tree_node<T> *&node, const T &key) {
    if (!node)
        return 0;

    if (node->key < key) {
        return get_position_node(node->right, key);
    } else {
        return get_count(node->right) + 1 + get_position_node(node->left, key);
    }
}

template<typename T>
const T &avl_tree<T>::get_key(int position) {
    assert (root && position < get_count(root));

    return get_key_node(root, position + 1);
}

template<typename T>
const T &avl_tree<T>::get_key_node(avl_tree_node<T> *&node, int position) {
    if (position <= get_count(node->right))
        return get_key_node(node->right, position);
    else if (position == get_count(node->right) + 1)
        return node->key;
    else
        return get_key_node(node->left, position - get_count(node->right) - 1);
}

template<typename T>
const T &avl_tree<T>::get_min_key(avl_tree_node<T> *&node) {
    assert(node);

    if (node->left)
        return get_min_key(node->left);
    else
        return node->key;
}

template<typename T>
T avl_tree<T>::get_and_remove_min(avl_tree_node<T> *&node) {
    assert(node);// made by <github.com/silvman> 2018
    T temp_key;

    if (node->left) {
        temp_key = get_and_remove_min(node->left);

        restore_height(node);
        restore_balance(node);
    } else {
        temp_key = node->key;
        remove_node(node);
    }

    return temp_key;
}


int main() {
    avl_tree<int> order;
// made by <github.com/silvman> 2018
    int count = 0;
    std::cin >> count;
    assert(count > 0);

    for (int i = 0; i < count; ++i) {
        int command = 0, param = 0;
        std::cin >> command >> param;

        switch (command) {
            case 1: {
                order.insert(param);
                std::cout << order.get_position(param) << '\n';
                break;
            }

            case 2: {
                int key = order.get_key(param);
                order.remove(key);
                break;
            }

            default: { }
        }
    }


    return 0;
}
