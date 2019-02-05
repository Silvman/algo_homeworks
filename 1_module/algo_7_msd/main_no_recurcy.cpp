// <github.com/silvman> 
// not msd
#include <iostream>
#include <string>
#include <stdexcept>

#define ALPHABET_SIZE 256
#define BUF_SIZE 128
#define BUF_MULT 2

template <typename T>
struct simple_vector {
    int size;
    int count;
    T* buffer;


    T& operator [] (int index);
    void add(const T& element);
    simple_vector();
    ~simple_vector();

private:
    void expand_buf();
};

template<typename T>
T &simple_vector<T>::operator[](int index) {
    if (index >= count || index < 0)
        throw std::out_of_range("no such index");

    return buffer[index];
}

template<typename T>
void simple_vector<T>::add(const T &element) {
    if (size == count)
        expand_buf();

    buffer[count] = element;
    count++;
}

template<typename T>
void simple_vector<T>::expand_buf() {
    int new_size = size * BUF_MULT;
    auto new_buffer = new T[new_size];
    for (int i = 0; i < count; ++i)
        new_buffer[i] = buffer[i];

    delete[] buffer;
    size = new_size;
    buffer = new_buffer;
}

template<typename T>
simple_vector<T>::simple_vector() : count(0), size(BUF_SIZE) {
    buffer = new T[size];
}

template<typename T>
simple_vector<T>::~simple_vector() {
    delete[] buffer;
}


void test_print(std::string* array, int count) {
    for (int i = 0; i < count; ++i)
        std::cout << array[i] << "\n";

    std::cout << "===========" << std::endl;
}

bool fastcmp (const std::string& a, const std::string& b, int n) {
    for (int i = 0; i < n; ++i) {
        if (a[i] != b[i])
            return false;
    }

    return true;
}
// <github.com/silvman>
void MSD_sort(std::string* array, int l, int r) {
    int count = r - l + 1;

    bool is_sorted = false;
    unsigned current_index = 0;
    while (!is_sorted) {
        is_sorted = true;

        auto buf_array = new std::string[count];
        for (int i = 0; i < count; ) { // пробег один раз
            if (current_index >= array[i].length()) {
                buf_array[i] = array[i];
                ++i;
            } else {
                int start_i = i;
                std::string& begins_with = array[start_i]; // будет ли он?

                auto positions = new int[ALPHABET_SIZE]();
                while( i < count
                       && (current_index <= array[i].length())
                       && (current_index == 0 || fastcmp(array[i], begins_with, current_index))) {
                    is_sorted = false;
                    ++positions[array[i][current_index]];
                    ++i;
                } // учесть, что строка может кончиться

                if (start_i == i) {
                    buf_array[i] = array[i];
                    ++i;
                } else {
                    for (int k = 1; k < ALPHABET_SIZE; ++k)
                        positions[k] += positions[k - 1];

                    for (int j = i - 1; j >= start_i; --j) { // TODO учесть сдвиг
                        buf_array[start_i + positions[array[j][current_index]] - 1] = array[j];
                        positions[array[j][current_index]]--;
                    }
                }
                // место для копирования

                delete[] positions;
            }
        }

        for (int j = 0; j < count; ++j)
            array[j] = buf_array[j];

        current_index++;
        delete[] buf_array;
    }
}

int main() {
    simple_vector<std::string> strector;
    while (!std::cin.eof()) {
        std::string buf;
        std::cin >> buf;
        if(buf[0] == '\0')
            break;

        strector.add(buf);
    }

    MSD_sort(strector.buffer, 0, strector.count - 1);

    for (int i = 0; i < strector.count; ++i)
        std::cout << strector[i] << std::endl;
// <github.com/silvman>
    return 0;
}
