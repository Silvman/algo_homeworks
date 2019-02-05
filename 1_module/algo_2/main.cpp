// made by <github.com/silvman> 2018
#include <iostream>
#include <stdexcept>

/*
 * Дан отсортированный массив различных целых чисел A[0..n-1]
 * и массив целых чисел B[0..m-1]. Для каждого элемента массива B[i]
 * найдите минимальный индекс элемента массива A[k], ближайшего
 * по значению к B[i]. n ≤ 110000, m ≤ 1000.
 */

/*
 * Исправил массивы - теперь динамические
 * Учитывается случай, когда бинпоиск возвращает 0
 *
 */

int searchByProbing(const int* array, int lenArray, int seekNum);
int binSearch(const int *array, int left, int right, int seekNum);
int operate(int* outputArr, int* sortedArr, int lenSortedArr, int* numArr, int lenNumArr);

int main() {
    try {
        int lenSortedArr;
        std::cin >> lenSortedArr;
        if (lenSortedArr < 1)
            throw std::length_error("Sorted array length is less than 0");

        auto sortedArr = new int[lenSortedArr];
        for (int j = 0; j < lenSortedArr; ++j)
            std::cin >> sortedArr[j];

        int lenNumArr;
        std::cin >> lenNumArr;
        if (lenNumArr < 1) {
            delete[] sortedArr;
            throw std::length_error("Numbers array length is less than 0");
        }

        auto numArr = new int[lenNumArr];
        for (int k = 0; k < lenNumArr; ++k)
            std::cin >> numArr[k];

        auto outputArr = new int[lenNumArr];
        operate(outputArr, sortedArr, lenSortedArr, numArr, lenNumArr);

        for (int i = 0; i < lenNumArr; ++i)
            std::cout << outputArr[i] << " ";

        delete[] sortedArr;
        delete[] outputArr;
        delete[] numArr;
    } catch(const std::exception& e) {
        std::cout << e.what();
    }

    std::cout << std::endl;

    return 0;
}


int searchByProbing(const int *array, int lenArray, int seekNum) {
    int indexBigger = 1;

    while ((indexBigger < lenArray) && (array[indexBigger] < seekNum))
        indexBigger = indexBigger * 2;

    if (indexBigger >= lenArray) {
        return lenArray - 1;
    }

    return indexBigger;
}

int binSearch(const int *array, int left, int right, int seekNum) {
    while (left < right) {
        int middle = (left + right) / 2;

        if (array[middle] < seekNum) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }

    return left;
}

int operate(int *outputArr, int *sortedArr, int lenSortedArr, int *numArr, int lenNumArr) {
    for (int i = 0; i < lenNumArr; ++i) {
        if (numArr[i] <= sortedArr[0]) {
            outputArr[i] = 0;
            continue;
        }

        if (numArr[i] >= sortedArr[lenSortedArr - 1]) {
            outputArr[i] = lenSortedArr - 1;
            continue;
        }

        int n = searchByProbing(sortedArr, lenSortedArr, numArr[i]); // первое приближение - ищем по степеням двойки, O(log(k))
        n = binSearch(sortedArr, n / 2, n, numArr[i]); // ищем ближайший с помощью бинпоиска

        if (n <= 0) {
            outputArr[i] = 0;
            continue;
        }

        if (numArr[i] - sortedArr[n - 1] <= sortedArr[n] - numArr[i]) {
            outputArr[i] = n - 1;
        } else {
            outputArr[i] = n;
        }
    }

    return 0;
}
