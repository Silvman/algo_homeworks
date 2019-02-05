// made by <github.com/silvman> 2018
#include <iostream>
#include <stdexcept>

/*
 * 6_2. Реализуйте стратегию выбора опорного элемента “медиана трёх”.
 * Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.
 */

int partition(int* array, int l, int r, int pivot);
int find_statistics(int* array, int count, int k);
int get_mediana(const int* array, int l, int r);

int main() {
    try {
        int count = 0, k = 0;
        std::cin >> count >> k;
        if (count < 1 || k >= count)
            throw std::out_of_range("bad count or/and k");

        auto numbers = new int[count];
        for (int i = 0; i < count; ++i)
            std::cin >> numbers[i];

        std::cout << find_statistics(numbers, count, k);
        delete[] numbers;
    } catch (const std::exception& e) {
        std::cout << e.what();
    }

    std::cout << std::endl;
    return 0;
}

int partition(int *array, int l, int r, int pivot) {
    int i = r;
    int j = r;
    std::swap(array[pivot], array[l]);

    // разделяем массив на числа, большие pivot'a и меньшие или равные ему
    while ( j > l ) {
        if (array[j] > array[l]) {
            std::swap(array[j], array[i]);
            i--;
        }

        j--;
    }

    std::swap(array[l], array[i]);
    return i;
}

int find_statistics(int* array, int count, int k) {
    if (k >= count)
        throw std::invalid_argument("k cannot be bigger than or equal to count");

    int l = 0, r = count - 1;
    while (l != r) {
        int pivot = get_mediana(array, l, r);
        int position = partition(array, l, r, pivot);
        if (position == k) {
            break;
        } else {
            if (position > k) {
                r = position - 1; // порядковая статистика в левой части
            } else {
                l = position + 1; // в правой части
            }
        }
    }
    return array[k];

}

int get_mediana(const int *array, int l, int r) {
    int m = (r + l) / 2;
    if( (array[l] >= array[m] && array[m] >= array[r])
        || (array[r] >= array[m] && array[m] >= array[l]) )
        return m;

    if( (array[m] >= array[r] && array[r] >= array[l])
        || (array[l] >= array[r] && array[r] >= array[m]) )
        return r;

    if( (array[r] >= array[l] && array[l] >= array[m])
        || (array[m] >= array[l] && array[l] >= array[r]) )
        return l;
}
