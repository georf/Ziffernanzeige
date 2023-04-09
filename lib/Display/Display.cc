#include "Display.h"

void Display::init()
{
    pinMode(SR_DATA_PIN, OUTPUT);
    pinMode(SR_LOAD_PIN, OUTPUT);
    pinMode(SR_CLOCK_PIN, OUTPUT);
    pinMode(SR_CLOCK_PIN_OVER_OE, OUTPUT);
}

void Display::red(uint8_t status)
{
    _red_led = status;
    selectRegister(0, 0);
}

void Display::green(uint8_t status)
{
    _green_led = status;
    selectRegister(0, 0);
}

void Display::selectRegister(uint8_t line, uint8_t part)
{
    // prepare for writing
    if (part != 0)
    {
        digitalWrite(SR_CLOCK_PIN_OVER_OE, HIGH);
    }
    digitalWrite(SR_LOAD_PIN, LOW);

    // second register
    uint8_t data = 0;
    bitWrite(data, 7, _green_led);
    if (line == 1)
        bitWrite(data, part, true);
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, MSBFIRST, data);

    // first register
    data = 0;
    bitWrite(data, 7, _red_led);
    if (line == 0)
        bitWrite(data, part, true);
    shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, MSBFIRST, data);

    // finish writing
    digitalWrite(SR_LOAD_PIN, HIGH);
    digitalWrite(SR_CLOCK_PIN_OVER_OE, part != 0);
}

void Display::setOutput(uint8_t line, const char *characters, byte showPoints)
{
    // Serial.print(line);
    // Serial.print(": ");
    // Serial.println(characters);

    for (uint8_t i = 0; i < 5; i++)
    {
        // change registers only if changed
        if (currentOutput[line][i] != characters[i])
        {
            selectRegister(line, 6 - i);
            shiftData(char2Data(characters[i]));
            currentOutput[line][i] = characters[i];
        }
    }

    if (currentShowingPoints[line] != showPoints)
    {
        selectRegister(line, 1);
        shiftData(points2Data(showPoints));
        currentShowingPoints[line] = showPoints;
    }
    selectRegister(line, 0);
}

byte Display::char2Data(char character)
{
    //   - a -
    // |       |
    // b       c
    // |       |
    //   - d -
    // |       |
    // e       f
    // |       |
    //   - g -
    //

    switch (character)
    {
    //         0babcdefg0;
    case '0':
        return 0b11101110;
    case '1':
        return 0b00100100;
    case '2':
        return 0b10111010;
    case '3':
        return 0b10110110;
    case '4':
        return 0b01110100;
    case '5':
        return 0b11010110;
    case '6':
        return 0b11011110;
    case '7':
        return 0b10100100;
    case '8':
        return 0b11111110;
    case '9':
        return 0b11110110;
    case '~':
        return 0b10000000;
    case '-':
        return 0b00010000;
    case '_':
        return 0b00000010;
    case '[':
        return 0b01000000;
    case '{':
        return 0b00001000;
    case ']':
        return 0b00100000;
    case '}':
        return 0b00000100;
    case 'b':
        return 0b01011110;
    case 'c':
        return 0b00011010;
    case 'n':
        return 0b00011100;
    case 'o':
        return 0b00011110;
    case 't':
        return 0b10011010;
    case 'y':
        return 0b01110110;
    case 'e':
        return 0b11111010;
    case 's':
        return 0b11010110;
    default:
        return 0;
    }
}

byte Display::points2Data(byte points)
{
    byte out = 0;
    if (bitRead(points, 3))
        bitSet(out, 1);
    if (bitRead(points, 2))
        bitSet(out, 3);
    if (bitRead(points, 1))
        bitSet(out, 2);
    if (bitRead(points, 0))
        bitSet(out, 4);
    return out;
}

void Display::shiftData(byte data)
{
    uint8_t i;

    digitalWrite(SR_CLOCK_PIN_OVER_OE, HIGH);
    digitalWrite(SR_LOAD_PIN, LOW);
    for (i = 0; i < 8; i++)
    {
        digitalWrite(SR_DATA_PIN, !!(data & (1 << (7 - i))));

        digitalWrite(SR_CLOCK_PIN_OVER_OE, LOW);
        digitalWrite(SR_CLOCK_PIN_OVER_OE, HIGH);
    }
    digitalWrite(SR_LOAD_PIN, HIGH);
}