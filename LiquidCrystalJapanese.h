#ifndef LiquidCrystalJapanese_h
#define LiquidCrystalJapanese_h

#include <inttypes.h>
#include "Print.h"
#include "KatakanaValue.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_FUNCTIONSET 0x20
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_CURSOROFF 0x00
#define LCD_BLINKOFF 0x00

// flags for function set
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_5x8DOTS 0x00

/** An LCD interface that is able to print Japanese Katakana characters. */
class LiquidCrystalJapanese : public Print
{
public:
    LiquidCrystalJapanese(uint8_t rs, uint8_t enable,
                          uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
    void begin(uint8_t cols, uint8_t rows);

    void clear();
    void display();

    void setCursor(uint8_t, uint8_t);
    virtual size_t write(uint8_t);
    size_t print(const char[]);
    static void translate(const char[], char *);

    using Print::print;
    using Print::write;

private:
    void init(uint8_t rs, uint8_t enable,
              uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
    void send(uint8_t, uint8_t);
    void setRowOffsets(int row1, int row2, int row3, int row4);
    void write4bits(uint8_t);
    void pulseEnable();
    void command(uint8_t);
    static uint8_t getValueForKatakana(const char[]);

    uint8_t _rs_pin;     // LOW: command.  HIGH: character.
    uint8_t _enable_pin; // activated by a HIGH pulse.
    uint8_t _data_pins[4];

    uint8_t _displayfunction;
    uint8_t _displaycontrol;
    uint8_t _displaymode;

    uint8_t _numlines;
    uint8_t _row_offsets[4];
};

#endif