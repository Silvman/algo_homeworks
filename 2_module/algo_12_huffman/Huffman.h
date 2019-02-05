//
// Created by silvman on 12.05.18.
//

#ifndef ALGO12_RENEW_HUFFMAN_H
#define ALGO12_RENEW_HUFFMAN_H


struct IInputStream {
    virtual bool Read(unsigned char& value) {
        int temp_value = getchar();
        if (temp_value == EOF) {
            return false;
        } else {
            value = temp_value;
            return true;
        }
    }
};

struct IOutputStream {
    virtual void Write(unsigned char value) {
        std::cout << value;
    }
};

void Encode(IInputStream& original, IOutputStream& compressed);
void Decode(IInputStream& compressed, IOutputStream& original);

#endif //ALGO12_RENEW_HUFFMAN_H
