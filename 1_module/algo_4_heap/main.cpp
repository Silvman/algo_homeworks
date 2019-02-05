// made by <github.com/silvman> 2018 
#include <iostream>
#include <stdexcept>

/*
 * Вовочка ест фрукты из бабушкиной корзины.
 * В корзине лежат фрукты разной массы. Вовочка может поднять не более K грамм.
 * Каждый фрукт весит не более K грамм. За раз он выбирает несколько
 * самых тяжелых фруктов, которые может поднять одновременно, откусывает
 * от каждого половину и кладет огрызки обратно в корзину. Если фрукт весит
 * нечетное число грамм, он откусывает большую половину. Фрукт массы 1гр
 * он съедает полностью.
 *
 * Определить за сколько подходов Вовочка съест все фрукты в корзине.
 *
 * Формат ввода
 * Вначале вводится n - количество фруктов и n строк с массами фруктов. n ≤ 50000.
 * Затем K - "грузоподъемность". K ≤ 1000.
 */

/*
 * пофиксено:
 * Куча изменяемого размера
 * Конструктор кучи из массива
 * обработка исключительных ситуаций (пустая куча, неправильные аргументы)
 * массивы неконстантного размера в динамической памяти
 */

#define BUF_SIZE 6
#define BUF_MULT 2

template <typename T>
class heap {
private:
    T* buf_heap;
    int buf_size;
    int top;

    void shift_up(int index);
    void shift_down(int index);
    void expand_buf();

public:
    explicit heap(int size = BUF_SIZE);
    heap(const T* array_from, int size);
    virtual ~heap();

    T get_max();
    const T& see_max() const;
    bool is_empty() const;
    void add_to_heap(const T& element);
};

int count_takes(heap<int>& fruits, int max_mass, int count);

int main() {
    try {
        int count = 0, max_mass = 0;
        std::cin >> count;
        if (count < 1)
            throw std::out_of_range("Count is not enough");

        auto fruit_buf = new int[count];
        for (int i = 0; i < count; ++i)
            std::cin >> fruit_buf[i];

        heap<int> fruits(fruit_buf, count);
        delete[] fruit_buf;

        std::cin >> max_mass;
        std::cout << count_takes(fruits, max_mass, count);
    } catch (const std::exception& e) {
        std::cout << e.what();
    }

    return 0;
}

template<typename T>
void heap<T>::add_to_heap(const T &element) {
    if (top + 1 == buf_size)
        expand_buf();

    buf_heap[top + 1] = element;

    if (!this->is_empty()) {
        this->shift_up(top + 1);
    }

    top++;
}

template<typename T>
void heap<T>::shift_up(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if ( buf_heap[index] <= buf_heap[parent] )
            return;

        std::swap(buf_heap[index], buf_heap[parent]);
        index = parent;
    }
}

template<typename T>
void heap<T>::shift_down(int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int largest = index;

    // Ищем большего сына, если такой есть.
    if( left <= this->top && this->buf_heap[left] > this->buf_heap[index] )
        largest = left;
// <github.com/silvman>
    if( right <= this->top && this->buf_heap[right] > this->buf_heap[largest] )
        largest = right;

    if( largest != index ) { // Если больший сын есть, то проталкиваем корень в него.
        std::swap( this->buf_heap[index], this->buf_heap[largest] );
        this->shift_down( largest );
    }
}

template<typename T>
bool heap<T>::is_empty() const {
    return top == -1;
}

template<typename T>
heap<T>::~heap() {
    delete[] buf_heap;
}

template<typename T>
heap<T>::heap(const int size) : buf_size(size), top(-1) {
    if(size < 1)
        throw std::invalid_argument("Wrong size");

    buf_heap = new T[buf_size];
}

template<typename T>
T heap<T>::get_max() {
    if (!this->is_empty()) {
        T temp = buf_heap[0];
        std::swap(buf_heap[0], buf_heap[top]);
        top--;
        this->shift_down(0);
        return temp;
    } else {
        throw std::out_of_range("Heap is empty");
    }
}

template<typename T>
const T &heap<T>::see_max() const {
    if (!this->is_empty())
        return buf_heap[0];
    else
        throw std::out_of_range("Heap is empty");
}

template<typename T>
void heap<T>::expand_buf() {
    int new_buf_size = buf_size * BUF_MULT;
    auto new_buf_heap = new T[new_buf_size];

    for (int i = 0; i < buf_size; ++i)
        new_buf_heap[i] = buf_heap[i];
// <github.com/silvman>
    delete[] buf_heap;
    buf_size = new_buf_size;
    buf_heap = new_buf_heap;
}

template<typename T>
heap<T>::heap(const T *array_from, int size) : buf_size(size), top(size - 1) {
    buf_heap = new T[size];
    for (int i = 0; i < size; ++i)
        buf_heap[i] = array_from[i];

    for (int i = size/2 - 1; i >= 0; --i)
        shift_down(i);
}


int count_takes(heap<int> &fruits, int max_mass, int count) {
    if(max_mass < 1 || fruits.is_empty())
        throw std::invalid_argument("Your arguments are invalid");

    int counter = 0;
    while (!fruits.is_empty()) {
        counter++;
        int capable_mass = max_mass;
        auto taken_fruits = new int[count];
        int count_of_fruits = 0;

        while(!fruits.is_empty() && capable_mass >= fruits.see_max()) {
            taken_fruits[count_of_fruits] = fruits.get_max();
            count_of_fruits++;
            capable_mass -= taken_fruits[count_of_fruits - 1];
        }

        for (int i = 0; i < count_of_fruits; ++i)
            if (taken_fruits[i] != 1)
                fruits.add_to_heap(taken_fruits[i] /= 2);

        delete[] taken_fruits;
    }
// <github.com/silvman>
    return counter;
}
