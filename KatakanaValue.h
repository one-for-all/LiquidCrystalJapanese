#ifndef KatakanaValue_h
#define KatakanaValue_h

struct KatakanaValue
{
    const char katakana[4];
    uint8_t value;
};

const KatakanaValue KATAKANA_VALUE_LIST[] = {
    // column 1
    {"。", 0b10100001},
    {"「", 0b10100010},
    {"」", 0b10100011},
    {"、", 0b10100100},
    {"・", 0b10100101},
    {"ヲ", 0b10100110},
    {"ァ", 0b10100111},
    {"ィ", 0b10101000},
    {"ゥ", 0b10101001},
    {"ェ", 0b10101010},
    {"ォ", 0b10101011},
    {"ャ", 0b10101100},
    {"ュ", 0b10101101},
    {"ョ", 0b10101110},
    {"ッ", 0b10101111},
    // column 2
    {"ー", 0b10110000},
    {"ア", 0b10110001},
    {"イ", 0b10110010},
    {"ウ", 0b10110011},
    {"エ", 0b10110100},
    {"オ", 0b10110101},
    {"カ", 0b10110110},
    {"キ", 0b10110111},
    {"ク", 0b10111000},
    {"ケ", 0b10111001},
    {"コ", 0b10111010},
    {"サ", 0b10111011},
    {"シ", 0b10111100},
    {"ス", 0b10111101},
    {"せ", 0b10111110},
    {"ソ", 0b10111111},
    // column 3
    {"タ", 0b11000000},
    {"チ", 0b11000001},
    {"ツ", 0b11000010},
    {"テ", 0b11000011},
    {"ト", 0b11000100},
    {"ナ", 0b11000101},
    {"ニ", 0b11000110},
    {"ヌ", 0b11000111},
    {"ネ", 0b11001000},
    {"ノ", 0b11001001},
    {"ハ", 0b11001010},
    {"ヒ", 0b11001011},
    {"フ", 0b11001100},
    {"ヘ", 0b11001101},
    {"ホ", 0b11001110},
    {"マ", 0b11001111},
    // column 4
    {"ミ", 0b11010000},
    {"ム", 0b11010001},
    {"メ", 0b11010010},
    {"モ", 0b11010011},
    {"ヤ", 0b11010100},
    {"ユ", 0b11010101},
    {"ヨ", 0b11010110},
    {"ラ", 0b11010111},
    {"リ", 0b11011000},
    {"ル", 0b11011001},
    {"レ", 0b11011010},
    {"ロ", 0b11011011},
    {"ワ", 0b11011100},
    {"ン", 0b11011101},
};

#endif