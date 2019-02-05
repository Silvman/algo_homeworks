// made by <github.com/silvman> 2018
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

    buffer[count] = element; // <github.com/silvman>
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

void MSD_sort(std::string* array, int l, int r, int current = 0) {
    int count = r - l + 1;
    if (count <= 1)
        return;

    auto buf_array = new std::string[count];
    auto positions = new int[ALPHABET_SIZE]();

    int offset = 0; // offset нужен для учета строк, которые недостаточно длинны для данного <github.com/silvman> current. В дальнейшем эти строки в отдельный bucket не попадут
    for (int i = 0; i < count;) { // пробег один раз
        if (current >= array[l + i].length()) {
            buf_array[offset] = array[l + i];
            offset++;
            ++i;
        } else {
            while (i < count && current < array[l + i].length()) {
                ++positions[array[l + i][current]];
                ++i;
            }
        }
    }

    for (int k = 1; k < ALPHABET_SIZE; ++k)
        positions[k] += positions[k - 1];

    for (int j = count - 1; j >= 0; --j) { // TODO учесть сдвиг
        if (current < array[l + j].length()) {
            buf_array[offset + positions[array[l + j][current]] - 1] = array[l + j];
            positions[array[l + j][current]]--;
        }
    }

    for (int j = 0; j < count; j++)
        array[l + j] = buf_array[j];

    delete[] buf_array;

    for (int m = 1; m < ALPHABET_SIZE; ++m)
        if (positions[m] != positions[m - 1]) // вызываем MSD для каждого образовавшегося bucket'a
            MSD_sort(array, l + offset + positions[m - 1], l + offset + positions[m] - 1, current + 1);
            // l - левая граница массива строк
            // + offset - учитываем кончившиеся строки
            // + positions[m - 1] - условный индекс начала bucket'a в массиве github.com/silvman
            // условный - потому что вычислен внутри конкретного bucket'a с учетом offset
            // positions[m] - 1 - условный индекс конца bucket'a

    if (l + offset + positions[ALPHABET_SIZE - 1] - 1 != r)
        MSD_sort(array, l + offset + positions[ALPHABET_SIZE - 1] - 1, r, current + 1);

    delete[] positions;
// <github.com/silvman>
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

    return 0;
}
