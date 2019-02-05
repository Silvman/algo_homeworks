// made by <github.com/silvman> 2018
#include <iostream>
#include <stdexcept>

#define BUF_SIZE 16
#define BUF_MULT 2

/*
 * Реализовать дек с динамическим зацикленным буфером.
 * Обрабатывать команды push * и pop *.
 *
 * Формат ввода
 * В первой строке количество команд n. n ≤ 1000000.
 * Каждая команда задаётся как 2 целых числа: a b.
 *
 * a = 1 - push front
 * a = 2 - pop front
 * a = 3 - push back
 * a = 4 - pop back
 *
 * Если дана команда pop *, то число b - ожидаемое значение.
 * Если команда pop * вызвана для пустой структуры данных, то ожидается “-1”.
 */

/*
 * исправил странное увеличение буфера
 * обработка пустого дека и не только с помощью исключений
 */

template<typename T>
class deque {
private:
    int lenBuf;
    int head;
    int tail;
    int count;
    T *buf;

    void expandBuf();

public:
    deque();
    ~deque();

    bool isEmpty();
    void pushBack(const T &elem);
    void pushFront(const T &elem);
    T popBack();
    T popFront();
};

void handle_operations (deque<int>& deque, int operation, int number) {
    switch (operation) {
        case 1: {
            deque.pushFront(number);
            break;
        }

        case 2: {
            try {
                if (deque.popFront() == number)
                    std::cout << "YES" << std::endl;
                else
                    std::cout << "NO" << std::endl;

            } catch (const std::out_of_range &e) {
                if (number == -1)
                    std::cout << "YES" << std::endl;
                else
                    std::cout << "NO" << std::endl;
            }

            break;
        }

        case 3: {
            deque.pushBack(number);
            break;
        }

        case 4: {
            try {
                if (deque.popBack() == number)
                    std::cout << "YES" << std::endl;
                else
                    std::cout << "NO" << std::endl;

            } catch (const std::out_of_range &e) {
                if (number == -1)
                    std::cout << "YES" << std::endl;
                else
                    std::cout << "NO" << std::endl;
            }

            break;
        }

        default:
            throw std::invalid_argument("unknown operation");
    }
}


int main() {
    try {
        int numOperations = 0;
        std::cin >> numOperations;
        if (numOperations < 1)
            throw std::out_of_range("Not enough operations");

        deque<int> numbers;
        for (int i = 0; i < numOperations; i++) {
            int operation, number;
            std::cin >> operation >> number;
            handle_operations(numbers, operation, number);
        }

    } catch (const std::exception &e) {
        std::cout << e.what();
    }

    return 0;
}


template<typename T>
void deque<T>::expandBuf() {
    int lenNewBuf = lenBuf * BUF_MULT;
    auto newBuf = new T[lenNewBuf];

    if (tail < head) { // копирование в случае, если голова ближе к концу буфера
        for (int i = head; i < lenBuf; ++i)
            newBuf[i - head] = buf[i]; // в новом буфере элементы имеют правильный порядок (голова левее хвоста)

        for (int i = 0; i <= tail; ++i)
            newBuf[lenBuf - head + i] = buf[i];
    } else {
        for (int i = 0; i < lenBuf; ++i)
            newBuf[i] = buf[i];
    }

    delete[] buf;

    head = 0; // после перестановки элементов голова оказывается в начале буфера,
    tail = lenBuf - 1; // хвост - последним иницилизированным
    lenBuf = lenNewBuf;
    buf = newBuf;
}

template<typename T>
deque<T>::~deque() {
    delete[] buf;
}

template<typename T>
deque<T>::deque() : lenBuf(BUF_SIZE), head(0), tail(0), count(0) {
    buf = new T[lenBuf];
}

template<typename T>
void deque<T>::pushBack(const T &elem) {
    if (tail + 1 < lenBuf) { // если дек может расти вправо
        if (tail + 1 == head) // дек заполнен
            expandBuf();

        tail++;
    } else { // если необходимо зациклиться
        if (head == 0) { // дек заполнен
            expandBuf();
            tail++;
        } else {
            tail = 0;
        }
    }

    if (isEmpty())
        head = tail; // один элемент - он одновременно голова и хвост

    count++;

    buf[tail] = elem;
}

template<typename T>
void deque<T>::pushFront(const T &elem) {
    if (head - 1 >= 0) { // если дек может расти влево
        if (head - 1 == tail) { // дек заполнен
            expandBuf();
            head = lenBuf - 1;
        } else {
            head--;
        }
    } else { // нужно зациклиться
        if (tail == lenBuf - 1) // дек заполнен
            expandBuf();

        head = lenBuf - 1;
    }

    if (isEmpty())
        tail = head;

    count++;

    buf[head] = elem;
}

template<typename T>
T deque<T>::popBack() {
    if (isEmpty())
        throw std::out_of_range("Deque is empty");

    T temp = buf[tail];

    if (tail - 1 >= 0) // если дек может уменьшаться влево
        tail--;
    else // если надо зациклиться
        tail = lenBuf - 1;

    count--;
    if (isEmpty())
        head = tail; // к исходному состоянию

    return temp;
}

template<typename T>
T deque<T>::popFront() {
    if (isEmpty())
        throw std::out_of_range("Deque is empty");

    T temp = buf[head];

    if (head + 1 < lenBuf) // если дек может уменьшаться вправо
        head++;
    else
        head = 0;

    count--;
    if (isEmpty())
        tail = head;

    return temp;
}

template<typename T>
bool deque<T>::isEmpty() {
    return count == 0;
}
