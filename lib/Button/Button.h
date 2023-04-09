#include "Arduino.h"

class Button
{
private:
    int lastValue;
    unsigned long lastDebounceTime;
    unsigned long nextLongPressedTime;
    void (*pCallback)();
    void (*pCallbackLong)();

public:
    int pin;
    int state;
    bool pressed;
    void read();
    Button(int PIN, void (*pCallbackPressed)(), void (*pCallbackLongPressed)());
};
