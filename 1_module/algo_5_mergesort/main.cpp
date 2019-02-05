// made by <github.com/silvman> 2018
#include <iostream>
#include <stdexcept>

/*
 *  На числовой прямой окрасили N отрезков.
 *  Известны координаты левого и правого концов каждого отрезка (Li и Ri).
 *  Найти длину окрашенной части числовой прямой.
 */

struct paint {
    int l, r;
    paint(int l = 0, int r = 0);
};

// in-place merge sort
template<typename T, typename compare>
void merge_sort(T *array, int l, int r, compare cmp);

bool paint_cmp(const paint& a, const paint& b);
int find(paint* array, int count);

int main() {
    try {
        int count;
        std::cin >> count;

        if(count < 1)
            throw std::length_error("wrong count");

        auto paints = new paint[count];
        for (int i = 0; i < count; ++i) {
            int left, right;
            std::cin >> left >> right;

            if (left > right)
                throw std::length_error("left cannot be righter than right");

            paints[i].l = left;
            paints[i].r = right;
        }

        std::cout << find(paints, count);
        delete[] paints;
    } catch (const std::exception& e) {
        std::cout << e.what();
    }

    std::cout << std::endl;
    return 0;
}


paint::paint(int l, int r) : l(l), r(r) { }

template<typename T, typename compare>
void merge_sort(T *array, int l, int r, compare cmp) {
    if (l > r || l < 0 || r < 0)
        throw std::invalid_argument("bad indexes");

    int count = r - l + 1;
    auto buf_array = new T[count];

    int subarray_size = 1; // размер отсортированного подмассива
    while (subarray_size < count) { // размер подмассива увеличивается <github.com/silvman> вдвое каждую итерацию, рано или поздно превысит count - тогда весь массив станет отсортированным подмассивом
        int i = 0, j = subarray_size, curr_index = 0;

        while (curr_index < count) { // мерджим по паре отсортированных подмассивов
            int start_i = i, start_j = j, start_index = curr_index;

            while (curr_index < count && curr_index - start_index != subarray_size * 2) {
                // cmp == array[i] <= array[j]
                if ( (j - start_j == subarray_size) // если j вышел за границы отсортированного подмассива
                     || j >= count // если j пробежал массив до конца (или подмассива не существует)
                     || ((i - start_i != subarray_size) && cmp(array[i], array[j]))
                        ) { // проверяем, не вышел ли i за границы отсортированного подмассива и делаем сравнение
                    buf_array[curr_index++] = array[i++];
                } else {
                    buf_array[curr_index++] = array[j++];
                }
            }

            // переходим к следующим подмассивам
            i += subarray_size;
            j += subarray_size;
        }

        for (int k = 0; k < count; ++k)
            array[l + k] = buf_array[k];

        subarray_size *= 2;
    }
// <github.com/silvman>
    delete[] buf_array;
}

int find(paint *array, int count) {
    merge_sort(array, 0, count - 1, paint_cmp);

    int first = array[0].l;
    int last = array[0].r;
    int length = 0;
    for (int i = 1; i < count; ++i) {
        if(array[i].l <= last + 1) {
            if (array[i].r > last)
                last = array[i].r;
        } else {
            length += last - first;
            first = array[i].l;
            last = array[i].r;
        }
    }
    length += last - first;

    return length;
}

bool paint_cmp(const paint &a, const paint &b) {
    return a.l <= b.l;
}// made by <github.com/silvman> 2018
