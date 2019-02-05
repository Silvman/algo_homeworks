// made by <github.com/silvman> 2018
#include <iostream>
#include <vector>
#include <cassert>
#include <stack>
#include <queue>

#include "Huffman.h"

#define HEAP_BUF_SIZE 6
#define HEAP_BUF_MULT 2

void Encode(IInputStream& original, IOutputStream& compressed);
void Decode(IInputStream& compressed, IOutputStream& original);

struct prefix_code {
    unsigned char symbol;
    std::vector<unsigned char> code;

    explicit prefix_code(unsigned char symbol = '\0') : symbol(symbol) { }
};

struct huffman_node {
    int             p;
    unsigned char   data;
    huffman_node    *left;
    huffman_node    *right;

    explicit huffman_node(unsigned char data = '\0') : data(data), p(1), left(nullptr), right(nullptr) { }

    bool operator >= (const huffman_node& other) {
        return p >= other.p;
    }

    bool operator < (const huffman_node& other) {
        return p < other.p;
    }
};


// Битовый поток. silvman. Для удобства работа с данными потоков.
class bit_stream {
public:
    bit_stream();

    void write_bit(unsigned char bit);
    void write_byte(unsigned char byte);

    unsigned char read_bit();
    unsigned char read_byte();

    const unsigned char* get_buffer() const;
    int get_bits_count() const;
    void set_bits_count(int count);

    void extract_last_bits_count();
    void write_last_bits_count();

    bool has_unread_bits();
    void start_again();

private:
    std::vector<unsigned char> buffer;
    int bits_count;
    unsigned char last_bits_count;
    int iterator;
};

bit_stream::bit_stream() : bits_count(0), iterator(0), last_bits_count(0) { }

void bit_stream::write_bit(unsigned char bit) {
    if(bits_count + 1 > buffer.size() * 8)
        buffer.push_back(0);

    if (bit != 0)
        buffer[bits_count / 8] |= 1 << (bits_count % 8);

    bits_count++;
    last_bits_count = bits_count % 8;
}

void bit_stream::write_byte(unsigned char byte) {
    if (bits_count % 8 == 0) {
        buffer.push_back(byte);
    } else {
        int offset = bits_count % 8;
        buffer[bits_count / 8] |= byte << offset;
        buffer.push_back(byte >> (8 - offset));
    }

    bits_count += 8;
    last_bits_count = bits_count % 8;
}

const unsigned char *bit_stream::get_buffer() const {
    return buffer.data();
}

int bit_stream::get_bits_count() const {
    return bits_count;
}

unsigned char bit_stream::read_byte() {
    assert(iterator < bits_count);

    unsigned char byte;
    if (iterator % 8 == 0) {
        byte = buffer[iterator / 8];
    } else {
        byte = buffer[iterator / 8] >> (iterator % 8); // TODO чтение с учетом сдвига!
        byte |= buffer[iterator / 8 + 1] << (8 - iterator % 8);
    }
    iterator += 8;

    return byte;
}

bool bit_stream::has_unread_bits() {
    return iterator < bits_count;
}

void bit_stream::start_again() {
    iterator = 0;
}

unsigned char bit_stream::read_bit() {
    assert(iterator < bits_count);
    unsigned char byte = (buffer[iterator / 8] >> iterator % 8) & 1;
    iterator++;
    return byte;
}

void bit_stream::write_last_bits_count() {
    buffer.push_back(last_bits_count);
    bits_count = buffer.size() * 8; // эмулирует чтение со стороннего потока
} // made by <github.com/silvman> 2018

void bit_stream::extract_last_bits_count() {
    last_bits_count = buffer.back();
    bits_count -= 16 - last_bits_count;
}

void bit_stream::set_bits_count(int count) {
    bits_count = count;

}

// специализированная куча для построения дерева Хаффмона
class heap {
private:
    huffman_node** buf_heap;
    int buf_size;
    int top;

    void shift_up(int index);
    void shift_down(int index);
    void expand_buf();

public:
    heap(huffman_node** array_from, int size);
    ~heap();

    huffman_node* get_min();
    bool is_empty() const;
    void add_to_heap(huffman_node*& element);
};

void heap::add_to_heap(huffman_node* &element) {
    if (top + 1 == buf_size)
        expand_buf();

    buf_heap[top + 1] = element;

    if (!this->is_empty()) {
        this->shift_up(top + 1);
    }

    top++;
}

void heap::shift_up(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if ( buf_heap[index]->p >= buf_heap[parent]->p )
            return;

        std::swap(buf_heap[index], buf_heap[parent]);
        index = parent;
    }
}

void heap::shift_down(int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int minimum = index; // made by <github.com/silvman> 2018

    // Ищем меньшего сына, если такой есть.
    if( left <= this->top && this->buf_heap[left]->p < this->buf_heap[index]->p )
        minimum = left;

    if( right <= this->top && this->buf_heap[right]->p < this->buf_heap[minimum]->p )
        minimum = right;

    if( minimum != index ) { // Если меньший сын есть, то проталкиваем корень в него.
        std::swap( this->buf_heap[index], this->buf_heap[minimum] );
        this->shift_down( minimum );
    }
}

bool heap::is_empty() const {
    return top == -1;
}

heap::~heap() {
    delete[] buf_heap;
}

huffman_node* heap::get_min() {
    assert (!this->is_empty());

    huffman_node *temp = buf_heap[0];
    std::swap(buf_heap[0], buf_heap[top]);
    top--;
    this->shift_down(0);
    return temp;
}

void heap::expand_buf() {
    int new_buf_size = buf_size * HEAP_BUF_MULT;
    auto new_buf_heap = new huffman_node* [new_buf_size];

    for (int i = 0; i < buf_size; ++i)
        new_buf_heap[i] = buf_heap[i];

    delete[] buf_heap;
    buf_size = new_buf_size;
    buf_heap = new_buf_heap;
}

heap::heap(huffman_node **array_from, int size) : buf_size(size), top(size - 1) {
    buf_heap = new huffman_node* [size];
    for (int i = 0; i < size; ++i)
        buf_heap[i] = array_from[i];

    for (int i = size/2 - 1; i >= 0; --i)
        shift_down(i);
}


void generate_code_table (huffman_node* huffman_tree, std::vector<unsigned char>& code, std::vector<prefix_code>& table) {
    if (!huffman_tree->right && !huffman_tree->left) {
        if (code.empty()) // случай дерева из одного элемента
            code.emplace_back(0);

        table.emplace_back(huffman_tree->data);
        table.back().code = code;
        return;
    }

    if (huffman_tree->left) {
        code.push_back(0);
        generate_code_table(huffman_tree->left, code, table);
    }

    code.pop_back();

    if (huffman_tree->right) {
        code.push_back(1);
        generate_code_table(huffman_tree->right, code, table);
    }

    code.pop_back();
}


void write_dictionary (bit_stream& stream, huffman_node* huffman_tree) {
    std::queue<huffman_node *> nodes;
    std::vector<unsigned char> symbols;

    nodes.push(huffman_tree);
    while (!nodes.empty()) {
        huffman_node *node = nodes.front();
        if (!node->left && !node->right){
            symbols.push_back(node->data);
        } else {
            nodes.push(node->left);
            nodes.push(node->right);
        }

        nodes.pop();
    }

    for (auto &&symbol : symbols) {
//        std::cout << symbol;
//        std::flush(std::cout);
// <github.com/silvman>
        stream.write_byte(symbol);
    }

    nodes.push(huffman_tree);
    while (!nodes.empty()) {
        huffman_node *node = nodes.front();
        if (!node->left && !node->right){
            stream.write_bit('\1');
        } else {
            stream.write_bit('\0');
            nodes.push(node->left);
            nodes.push(node->right);
        }

        nodes.pop();
    }



}

huffman_node* read_dictionary (bit_stream& stream, int count) {
    std::queue<huffman_node *> nodes;
    std::vector<unsigned char> symbols;

    count = (count ? count : 256); // число нод silvman
    for (int i = 0; i < count; ++i) {
        symbols.push_back(stream.read_byte());
    }

    int count_nodes = count * 2 - 1;
    int read_symbols = 0;

    auto *tree = new huffman_node;
    unsigned char bit = stream.read_bit();
    if (bit == '\1') {
        tree->data = symbols[read_symbols];
    } else {
        nodes.push(tree);
        for (int j = 0; j < count_nodes - 1; ++j) {
            auto *parent = nodes.front();

            auto *node = new huffman_node;
            if (parent)
                if (!parent->left) {
                    parent->left = node;
                } else {
                    parent->right = node;
                }

            unsigned char bit = stream.read_bit();
            if (bit == '\1') {
                node->data = symbols[read_symbols];
                read_symbols++;
            } else {
                nodes.push(node);
            }

            if (parent->left && parent->right)
                nodes.pop();
        }
    }

    return tree;
}

void free_dictionary(huffman_node* huffman_tree) {
    if (huffman_tree->left)
        free_dictionary(huffman_tree->left);

    if(huffman_tree->right)
        free_dictionary(huffman_tree->right);

    delete huffman_tree;
}

// дешифровка
void decode_bit_stream(bit_stream &encoded, bit_stream &original) {
    encoded.start_again();
    char is_encoded = encoded.read_bit();

    if (is_encoded == '\1') {
        encoded.extract_last_bits_count();

        int num_alphabet = 0;  // число символов
        num_alphabet |= encoded.read_byte();

        huffman_node* read_tree = read_dictionary(encoded, num_alphabet);
        std::vector<prefix_code> read_table;
        std::vector<unsigned char> read_code;

        generate_code_table(read_tree, read_code, read_table);

        while (encoded.has_unread_bits()) {
            std::vector<unsigned char> sequence;

            bool is_equal = false;
            while (!is_equal) {
                sequence.push_back(encoded.read_bit());
                for (auto &&item : read_table) {
                    if (item.code.size() != sequence.size())
                        continue;

                    for (int i = 0; i < sequence.size(); ++i) {
                        if(sequence[i] == item.code[i]) {
                            is_equal = true;
                        } else {
                            is_equal = false;
                            break;
                        }
                    }

                    if (is_equal) {
                        original.write_byte(item.symbol);
                        break;
                    }
                }
            }
        }

        free_dictionary(read_tree);
    } else {
        encoded.set_bits_count(encoded.get_bits_count() - 7);

        while (encoded.has_unread_bits())
            original.write_byte(encoded.read_byte());
    }
}

void encode_bit_stream(bit_stream &original, bit_stream &encoded) {
    // составляем вектор всех используемых символов todo silvman м.б. следует использовать сет?
    std::vector<huffman_node*> symbols;
    while (original.has_unread_bits()) {
        unsigned char symbol = original.read_byte();
        bool is_in_vector = false;
        for (auto &&item : symbols) {
            if (item->data == symbol) {
                item->p++;
                is_in_vector = true;
                break;
            }
        }

        if (!is_in_vector)
            symbols.push_back(new huffman_node(symbol));
    }


    heap tree_builder(symbols.data(), (int)symbols.size());
    huffman_node* huffman_tree = nullptr;
    while (!tree_builder.is_empty()) {
        huffman_node *left = tree_builder.get_min();
        if (tree_builder.is_empty()) {
            huffman_tree = left;
            break;
        }

        huffman_node *right = tree_builder.get_min();

        auto *combine   = new huffman_node;
        combine->p      = left->p + right->p;
        combine->left   = left;
        combine->right  = right;

        tree_builder.add_to_heap(combine);
    }

    // генерируем префиксный код

    std::vector<unsigned char> code_buffer;
    std::vector<prefix_code> table;

    generate_code_table(huffman_tree, code_buffer, table);

    /* --- кодирование --- */

    original.start_again();

    auto num_symbols = (unsigned char)table.size();
    encoded.write_bit('\1'); // закодировано или нет ;)
    encoded.write_byte(num_symbols); // запись числа символов
    write_dictionary(encoded, huffman_tree); // запись словаря
// <github.com/silvman>
    while(original.has_unread_bits()) { // запись сообщения
        unsigned char symbol = original.read_byte();

        for (auto &&code : table)
            if (code.symbol == symbol)
                for (auto &&bit : code.code)
                    encoded.write_bit(bit);

    }

    free_dictionary(huffman_tree);
    encoded.write_last_bits_count();

    if (original.get_bits_count() + 1 < encoded.get_bits_count()) {
        bit_stream pseudo_encoded;
        pseudo_encoded.write_bit('\0');
        original.start_again();
        while (original.has_unread_bits())
            pseudo_encoded.write_byte(original.read_byte());

        encoded = pseudo_encoded;
    }
}


static void copyStream(IInputStream& input, IOutputStream& output) {
    unsigned char value;
    while (input.Read(value)) {
        output.Write(value);
    }
}

void Encode(IInputStream& original, IOutputStream& compressed) {
    bit_stream input_stream;
    bit_stream encoded_stream;

    unsigned char value;
    while(original.Read(value))
        input_stream.write_byte(value);

    encode_bit_stream(input_stream, encoded_stream);

    while (encoded_stream.has_unread_bits())
        compressed.Write(encoded_stream.read_byte());
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    bit_stream encoded_stream;
    bit_stream output_stream;

    unsigned char value;
    while(compressed.Read(value))
        encoded_stream.write_byte(value);

    decode_bit_stream(encoded_stream, output_stream);

    output_stream.start_again();

    while (output_stream.has_unread_bits())
        original.Write(output_stream.read_byte());
}

int main(int argc, char **argv) {
    bit_stream input_stream;
    bit_stream encoded_stream;
    bit_stream output_stream;

    int c;
    while((c = getchar()) != '\n')
        input_stream.write_byte(c);

    encode_bit_stream(input_stream, encoded_stream);
    decode_bit_stream(encoded_stream, output_stream);

    while (output_stream.has_unread_bits())
        std::cout << output_stream.read_byte();

    std::cout << std::endl << "size before: " << input_stream.get_bits_count() << '\n';
    std::cout << "size encoded: " << encoded_stream.get_bits_count() << '\n';
    std::cout << "compression: " << (float) encoded_stream.get_bits_count() / input_stream.get_bits_count() << '\n';
    std::cout << "whole: " << (float) output_stream.get_bits_count() / input_stream.get_bits_count() << '\n';

//    IInputStream input;
//    IOutputStream output;
//    github silvman;
//
//    if (argv[1][0] == 'c')
//        Encode(input, output);
//
//    if (argv[1][0] == 'd')
//        Decode(input, output);
    return 0;
}
