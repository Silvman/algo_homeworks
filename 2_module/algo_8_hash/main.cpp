// made by <github.com/silvman> 2018
#include <iostream>
#include <vector>
#include <cassert>

/*
 * Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
 * Хранимые строки непустые и состоят из строчных латинских букв.
 * Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.

 * Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
 * когда коэффициент заполнения таблицы достигает 3/4.
 *
 * Структура данных должна поддерживать операции
 * добавления строки в множество,
 * удаления строки из множества
 * проверки принадлежности данной строки множеству.
 * 1_1. Для разрешения коллизий используйте квадратичное пробирование.
 */

// + еще хэш-функция для интов

unsigned long hash_function (const std::string& key, unsigned long table_size) {
    unsigned long hash = 0;
    for (char i : key)
        hash = (hash * 127 + i) % table_size;

    return hash;
}

unsigned long hash_function (const int key, unsigned long table_size) {
    return key % table_size;
}

template <typename T>
class hash_table {
    std::vector<T> buffer;
    unsigned long count;

    const T del;
    const T nil;

    void grow();
    unsigned long probe(unsigned long last_probe, int i, unsigned long size);

public:
    hash_table(unsigned long size, const T &nil, const T &del);
    ~hash_table() = default;

    bool has(const T& key);
    bool add(const T& key);
    bool remove(const T& key);
};


template<typename T>
hash_table<T>::hash_table(unsigned long size, const T &nil, const T &del) : count(0), nil(nil), del(del) {
    assert(size > 0);
    buffer.resize(size, nil);
}


template<typename T>
unsigned long hash_table<T>::probe(unsigned long last_probe, int i, unsigned long size) {
    return (last_probe + i + 1) % size;
}

template<typename T>
bool hash_table<T>::has(const T &key) {
    unsigned long hash = hash_function(key, buffer.size());
    for (int i = 0; buffer[hash] != nil && i != buffer.size(); i++) { //  i != buffer.size()  - поокрывает кейс когда вся таблица в silvman del'ах
        if (key == buffer[hash])
            return true;

        hash = probe(hash, i, buffer.size());
    }

    return false;
}

template<typename T>
bool hash_table<T>::add(const T &key) {
    if (count * 4 >= buffer.size() * 3) // коэфициент переполнения 3/4
        grow();

    unsigned long hash = hash_function(key, buffer.size());
    unsigned long deleted_hash = 0;
    bool has_deleted_hash = false;

    for (int i = 0; buffer[hash] != nil && i != buffer.size(); i++) {
        if(buffer[hash] == key) // элемент существует
            return false;

        if (buffer[hash] == del && !has_deleted_hash) {
            deleted_hash = hash;
            has_deleted_hash = true;
        }

        hash = probe(hash, i, buffer.size());
    }

    if (has_deleted_hash) {
        buffer[deleted_hash] = key; // вставка по месту удаленного
    } else {
        buffer[hash] = key;
    }

    count++;

    return true;
}


template<typename T>
bool hash_table<T>::remove(const T &key) {
    unsigned long hash = hash_function(key, buffer.size());
    for (int i = 0; buffer[hash] != nil && i != buffer.size(); i++) {
        if(buffer[hash] == key) {
            buffer[hash] = del;
            count--;
            return true;
        }

        hash = probe(hash, i, buffer.size());
    }

    return false; // элемента не существует

}

template<typename T>
void hash_table<T>::grow() {
    std::vector<T> new_buffer;
    new_buffer.resize(buffer.size() * 2, nil);

    for (int i = 0; i < buffer.size(); ++i) {
        if (buffer[i] != nil && buffer[i] != del) {
            unsigned long hash = hash_function(buffer[i], new_buffer.size());
            for (int j = 0; new_buffer[hash] != nil; ++j)
                hash = probe(hash, j, new_buffer.size());

            new_buffer[hash] = buffer[i];
        }
    }

    buffer = new_buffer;
}

int main() {
//    hash_table<int> table(43, -1, -2);// made by <github.com/silvman> 2018
    hash_table<std::string> table(8, "NIL", "DEL");

    char operation = '\0';
//    int word = 0;
    std::string word;

    while ( std::cin >> operation >> word ) {
        if (operation == '+')
            std::cout << (table.add(word) ? "OK" : "FAIL") << '\n';

        if (operation == '-')
            std::cout << (table.remove(word) ? "OK" : "FAIL") << '\n';

        if (operation == '?')
            std::cout << (table.has(word) ? "OK" : "FAIL") << '\n';
    }

    return 0;
}// made by <github.com/silvman> 2018
