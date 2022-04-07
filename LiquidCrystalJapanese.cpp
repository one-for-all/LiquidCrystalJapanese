#include "LiquidCrystalJapanese.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

LiquidCrystalJapanese::LiquidCrystalJapanese(uint8_t rs, uint8_t enable,
                                             uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
    init(rs, enable, d0, d1, d2, d3);
}

void LiquidCrystalJapanese::init(uint8_t rs, uint8_t enable,
                                 uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
    _rs_pin = rs;
    _enable_pin = enable;

    _data_pins[0] = d0;
    _data_pins[1] = d1;
    _data_pins[2] = d2;
    _data_pins[3] = d3;

    _displayfunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;

    begin(/* cols= */ 16, /* lines= */ 2);
}

void LiquidCrystalJapanese::begin(uint8_t cols, uint8_t lines)
{
    if (lines > 1)
    {
        _displayfunction |= LCD_2LINE;
    }
    _numlines = lines;

    setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);

    pinMode(_rs_pin, OUTPUT);
    pinMode(_enable_pin, OUTPUT);

    // Do these once, instead of every time a character is drawn for speed reasons.
    for (int i = 0; i < 4; ++i)
    {
        pinMode(_data_pins[i], OUTPUT);
    }

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
    delayMicroseconds(50000);
    // Now we pull both RS and R/W low to begin commands
    digitalWrite(_rs_pin, LOW);
    digitalWrite(_enable_pin, LOW);

    // Put the LCD into 4 bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46
    // we start in 8bit mode, try to set 4 bit mode
    write4bits(0x03);
    delayMicroseconds(4500); // wait min 4.1ms

    // second try
    write4bits(0x03);
    delayMicroseconds(4500); // wait min 4.1ms

    // third go!
    write4bits(0x03);
    delayMicroseconds(150);

    // finally, set to 4-bit interface
    write4bits(0x02);

    // finally, set # lines, font size, etc.
    command(LCD_FUNCTIONSET | _displayfunction);

    // turn the display on with no cursor or blinking default
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();

    // clear it off
    clear();

    // Initialize to default text direction (for romance languages)
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // set the entry mode
    command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystalJapanese::setRowOffsets(int row0, int row1, int row2, int row3)
{
    _row_offsets[0] = row0;
    _row_offsets[1] = row1;
    _row_offsets[2] = row2;
    _row_offsets[3] = row3;
}

/********** high level commands, for the user! */
void LiquidCrystalJapanese::clear()
{
    command(LCD_CLEARDISPLAY); // clear display, set cursor position to zero
    delayMicroseconds(2000);   // this command takes a long time!
}

void LiquidCrystalJapanese::display()
{
    _displaycontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystalJapanese::setCursor(uint8_t col, uint8_t row)
{
    const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
    if (row >= max_lines)
    {
        row = max_lines - 1;
    }
    if (row >= _numlines)
    {
        row = _numlines - 1;
    }

    command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

size_t LiquidCrystalJapanese::print(const char str[])
{
    const size_t size = strlen(str);
    size_t cur = 0;
    const uint8_t *buffer = (const uint8_t *)str;
    while (size - cur >= 3)
    {
        char katakana[4];
        strncpy(katakana, str + cur, 3);
        katakana[3] = '\0';
        uint8_t value = getValueForKatakana(katakana);
        if (value != 0) // Detected Katakana
        {
            cur += 3;
            buffer += 3;
            write(value);
        }
        else
        {
            cur++;
            write(*buffer++);
        }
    }
    while (size - cur >= 1)
    {
        cur++;
        write(*buffer++);
    }
    return cur;
}

uint8_t LiquidCrystalJapanese::getValueForKatakana(const char katakana[])
{
    for (uint8_t i = 0; i < sizeof(KATAKANA_VALUE_LIST) / sizeof(KatakanaValue); ++i)
    {
        if (strcmp(katakana, KATAKANA_VALUE_LIST[i].katakana) == 0)
            return KATAKANA_VALUE_LIST[i].value;
    }
    return 0;
}

/*********** mid level commands, for sending data/cmds */

inline void LiquidCrystalJapanese::command(uint8_t value)
{
    send(value, LOW);
}

inline size_t LiquidCrystalJapanese::write(uint8_t value)
{
    send(value, HIGH);
    return 1; // assume sucess
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void LiquidCrystalJapanese::send(uint8_t value, uint8_t mode)
{
    digitalWrite(_rs_pin, mode);

    write4bits(value >> 4);
    write4bits(value);
}

void LiquidCrystalJapanese::write4bits(uint8_t value)
{
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(_data_pins[i], (value >> i) & 0x01);
    }

    pulseEnable();
}

void LiquidCrystalJapanese::pulseEnable(void)
{
    digitalWrite(_enable_pin, LOW);
    delayMicroseconds(1);
    digitalWrite(_enable_pin, HIGH);
    delayMicroseconds(1); // enable pulse must be >450ns
    digitalWrite(_enable_pin, LOW);
    delayMicroseconds(100); // commands need > 37us to settle
}