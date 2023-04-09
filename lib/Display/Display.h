#include "Arduino.h"

#define SR_DATA_PIN D8          // Outputs the byte to transfer
#define SR_LOAD_PIN D7          // Controls the internal transference of data in SN74HC595 internal registers
#define SR_CLOCK_PIN D6         // Generates the clock signal to control the transference of data
#define SR_CLOCK_PIN_OVER_OE D5 // Generates the clock signal to control the transference of data

class Display
{
private:
    // LED status
    uint8_t _red_led;
    uint8_t _green_led;

    // current output in registers
    char currentOutput[2][5] = {"", ""};
    byte currentShowingPoints[2] = {0, 0};

    void selectRegister(uint8_t line, uint8_t part);
    void shiftData(byte data);
    byte char2Data(char character);
    byte points2Data(byte points);

public:
    void init();
    void red(uint8_t status);
    void green(uint8_t status);
    void setOutput(uint8_t line, const char *characters, byte showPoints);
};
