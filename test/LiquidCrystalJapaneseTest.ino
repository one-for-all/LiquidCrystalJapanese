#line 2 "LiquidCrystalJapaneseTest.ino"

#include <AUnit.h>
#include <LiquidCrystalJapanese.h>

test(translate_singleJapaneseChar_correct)
{
    // Arrange.
    char kat[] = "ー";
    char result[sizeof(kat)];

    // Act.
    LiquidCrystalJapanese::translate(kat, result);

    // Assert.
    char expected[] = {char(0b10110000), '\0'};
    assertEqual(result, expected);
}

test(translate_japaneseMixedWithEnglish_correct)
{
    // Arrange.
    char kat[] = "レモンabc";
    char result[sizeof(kat)];

    // Act.
    LiquidCrystalJapanese::translate(kat, result);

    // Assert.
    char expected[] = {char(0b11011010), char(0b11010011), char(0b11011101), 'a', 'b', 'c', '\0'};
    assertEqual(result, expected);
}

void setup()
{
    delay(1000);          // wait for stability on some boards to prevent garbage Serial
    Serial.begin(115200); // ESP8266 default of 74880 not supported on Linux
    while (!Serial)
        ; // for the Arduino Leonardo/Micro only
}

void loop()
{
    aunit::TestRunner::run();
}