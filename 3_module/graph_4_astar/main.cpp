// made by <github.com/silvman> 2018
#include<queue>
#include<string>
#include<iostream>
#include<cassert>
#include<unordered_map>
#include<cstring>
#include <algorithm>

/*
    Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду:
    [ 1  2  3  4 ]
    [ 5  6  7  8 ]
    [ 9  10 11 12]
    [ 13 14 15 0 ]
    где 0 задает пустую ячейку.
    Достаточно найти хотя бы какое-то решение. Число перемещений костяшек не обязано быть минимальным.
    Формат входных данных
    Начальная расстановка.
    Формат выходных данных
    Если решение существует, то в первой строке выходного файла выведите минимальное число перемещений костяшек,
    которое нужно сделать, чтобы достичь выигрышной конфигурации, а во второй строке выведите соответствующую
    последовательность ходов: L означает, что костяшка сдвинулась влево, R – вправо, U – вверх, D – вниз. Если таких
    последовательностей несколько, то выведите любую из них. Если же выигрышная конфигурация недостижима, то выведите
    в выходной файл одно число −1.
 */

const char field_size = 16;
const char y_size = 4;
const char x_size = 4;
const std::array<char, field_size> finish_field = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 0
};

class game_state {
public:
    game_state(const std::array<char, field_size> &_field);

    bool is_complete() const;
    bool can_move_left() const;
    bool can_move_right() const;
    bool can_move_up() const;
    bool can_move_down() const;

    game_state move_left() const;
    game_state move_right() const;
    game_state move_up() const;
    game_state move_down() const;

    int heuristic() const;
    bool operator==(const game_state &state) const;

    friend struct state_hash;

private:
    int get_x(int pos) const;
    int get_y(int pos) const;

    std::array<char, field_size> field;
    char empty_pos;
};

game_state::game_state(const std::array<char, field_size> &_field) : field(_field) {
    empty_pos = -1;
    for (char i = 0; i < field_size; i++)
        if (field[i] == 0)
            empty_pos = i;

    assert(empty_pos != -1);
}

bool game_state::is_complete() const {
    return field == finish_field;
}

bool game_state::can_move_left() const {
    return empty_pos % x_size != x_size - 1;
}

bool game_state::can_move_right() const {
    return empty_pos % x_size != 0;
}

bool game_state::can_move_up() const {
    return empty_pos < field_size - x_size;
}

bool game_state::can_move_down() const {
    return empty_pos > x_size - 1;
}

game_state game_state::move_left() const {
    assert(can_move_left());

    game_state leftState(*this);
    std::swap(leftState.field[empty_pos], leftState.field[empty_pos + 1]);
    leftState.empty_pos++;
    return leftState;
}


game_state game_state::move_right() const {
    assert(can_move_right());

    game_state right_state(*this);
    std::swap(right_state.field[empty_pos], right_state.field[empty_pos - 1]);
    right_state.empty_pos--;
    return right_state;
}

game_state game_state::move_up() const {
    assert(can_move_up());

    game_state up_state(*this);
    std::swap(up_state.field[empty_pos], up_state.field[empty_pos + x_size]);
    up_state.empty_pos += x_size;
    return up_state;
}

game_state game_state::move_down() const {
    assert(can_move_down());

    game_state down_state(*this);
    std::swap(down_state.field[empty_pos], down_state.field[empty_pos - x_size]);
    down_state.empty_pos -= x_size;
    return down_state;
}

bool game_state::operator==(const game_state &state) const {
    return field == state.field;
}

struct state_hash {
    size_t operator()(const game_state &state) const {
        size_t hash = 0;
        memcpy(&hash, &(state.field[0]), sizeof(hash));
        return hash;
    }
};

int game_state::heuristic() const {
    int h = 0;
    for (char i = 1; i < field_size; ++i) {
        for (char j = 0; j < field_size; ++j) {
            if (field[j] == i) {
                h += abs(get_x(i - 1) - get_x(j)) + abs(get_y(i - 1) - get_y(j)); // число ходов до своего места
            }
        }
    }

    h += abs(get_x(empty_pos) - get_x(field_size - 1)) + abs(get_y(empty_pos) - get_y(field_size - 1)); // для пустой костяшки

    return h;
}

int game_state::get_x(int pos) const {
    return pos % x_size;
}

int game_state::get_y(int pos) const {
    return pos / y_size;
}

bool IsSolvable(const std::array<char, field_size> &field) {
    int inversion = 0; // число инверсий + строка, в которой ноль (считая с 1)
    int posEmpty = 0;
    for (int i = 0; i < field_size - 1; ++i) {
        for (int j = i + 1; j < field_size; ++j) {
            if (!field[i]) {
                posEmpty = i;
                continue;
            }

            if (!field[j]) {
                posEmpty = j;
                continue;
            }

            if (field[i] > field[j])
                inversion++;
        }
    }

    inversion += 1 + posEmpty / y_size;

    return !(inversion % 2);
}


struct bigger_heuristic {
    bool operator()(const game_state &a, const game_state &b) {
        return a.heuristic() > b.heuristic();
    }
};

std::string GetSolution(const std::array<char, field_size> &field) {
    std::unordered_map<game_state, char, state_hash> visited;
    game_state start_state(field);
    std::priority_queue<game_state, std::vector<game_state>, bigger_heuristic> queue; // очредь с приоритетом, вверху лучшая эвристика (самая маленькая)

    queue.push(start_state);
    visited[start_state] = 'S';
    while (true) {
        if (queue.size() > 100) { // если очередь слишком велика, удаляем варианты с большой эвристикой
            std::priority_queue<game_state, std::vector<game_state>, bigger_heuristic> new_queue;
            for (int i = 0; i < 50; ++i) {
                new_queue.push(queue.top());
                queue.pop();
            }
            queue = new_queue;
        }

        game_state state = queue.top();
        queue.pop();
        if (state.is_complete())
            break;

        if (state.can_move_left()) {
            game_state leftState = state.move_left();
            if (visited.find(leftState) == visited.end()) {
                queue.push(leftState);
                visited[leftState] = 'L';
            }
        }
        if (state.can_move_right()) {
            game_state rightState = state.move_right();
            if (visited.find(rightState) == visited.end()) {
                queue.push(rightState);
                visited[rightState] = 'R';
            }
        }
        if (state.can_move_up()) {
            game_state upState = state.move_up();
            if (visited.find(upState) == visited.end()) {
                queue.push(upState);
                visited[upState] = 'U';
            }
        }
        if (state.can_move_down()) {
            game_state downState = state.move_down();
            if (visited.find(downState) == visited.end()) {
                queue.push(downState);
                visited[downState] = 'D';
            }
        }
    }


    std::string result;
    game_state state(finish_field);
    while (visited[state] != 'S') {
        switch (visited[state]) {
            case 'L':
                state = state.move_right();
                result += 'L';
                break;
            case 'R':
                state = state.move_left();
                result += 'R';
                break;
            case 'U':
                state = state.move_down();
                result += 'U';
                break;
            case 'D':
                state = state.move_up();
                result += 'D';
                break;
            default:
                assert(false);
        }

    }

    std::reverse(result.begin(), result.end());
    return result;
}

int main() {
    std::array<char, field_size> field = {0};

    for (int i = 0; i < field_size; ++i) {
        int num;
        std::cin >> num;
        field[i] = num;
    }

    if (IsSolvable(field)) {
        std::string solution = GetSolution(field);
        std::cout << solution.size() << std::endl;
        std::cout << solution << std::endl;
    } else {
        std::cout << -1 << std::endl;
    }
    return 0;
}
