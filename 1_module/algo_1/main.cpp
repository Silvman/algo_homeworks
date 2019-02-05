// made by silvman <github.com/silvman> 2018
//
#include <iostream>

/*
 * Даны два строго возрастающих массива целых чисел A[0..n) и B[0..m) и число k.
 * Найти количество таких пар индексов (i, j), что A[i] + B[j] = k.
 * Время работы O(n + m). n, m ≤ 100000.
 *
 * Указание. Обходите массив B от конца к началу.
 */

int count(const int* firstArr, int lenFirstArr, const int* secondArr, int lenSecondArr, int seekSum) {
    int i = 0, counter = 0, j = lenSecondArr - 1;

    /*
     * Движемся по одному массиву с начала, по другому с конца. Движемся только в одном направлении.
     * Если в сумме на данных индексах получается число большее, чем seekSum, уменьшаем индекс для второго массива
     * - другим образом сумму не уменьшить.
     * Аналогично для случая, когда в сумме получается меньше, чем seekSum.
     */

    while (i < lenFirstArr && j >= 0) {
        if (firstArr[i] + secondArr[j] > seekSum) {
            j--;
        }

        if (firstArr[i] + secondArr[j] < seekSum) {
            i++;
        }

        if (firstArr[i] + secondArr[j] == seekSum) {
            counter++;
            j--; // просто чтобы сдвинуть с мертвой точки
        }
    }

    return counter;
}

int main() {
    int lenFirstArr, lenSecondArr, seekSum;

    std::cin >> lenFirstArr;
    int firstArr[lenFirstArr];
    for (auto &&item : firstArr) {
        std::cin >> item;
    }
// made by <github.com/silvman> 2018
    std::cin >> lenSecondArr;
    int secondArr[lenSecondArr];
    for (auto &&item : secondArr) {
        std::cin >> item;
    }

    std::cin >> seekSum;

    std::cout << count(firstArr, lenFirstArr, secondArr, lenSecondArr, seekSum);

    return 0;
}

